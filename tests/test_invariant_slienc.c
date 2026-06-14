#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

START_TEST(test_filename_extension_buffer_overflow)
{
    // Invariant: appending ".szp" extension must never overflow the filename buffer
    // The buffer size in slienc.c appears to be 256 bytes based on typical usage
    
    const char *payloads[] = {
        // Exact exploit: filename that leaves no room for ".szp" (4 bytes + null)
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",  // 254 chars
        // Boundary: exactly at buffer limit
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",  // 251 chars
        // Valid input: short filename
        "test_input.txt",
    };
    int num_payloads = sizeof(payloads) / sizeof(payloads[0]);

    for (int i = 0; i < num_payloads; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child: run slienc with the payload as filename argument
            execl("./tools/slienc", "slienc", payloads[i], (char *)NULL);
            _exit(127);  // exec failed
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
            // Security invariant: process must not crash due to buffer overflow
            // SIGSEGV or SIGABRT indicates memory corruption
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                ck_assert_msg(sig != SIGSEGV && sig != SIGABRT && sig != SIGBUS,
                    "Buffer overflow detected with payload %d (signal %d)", i, sig);
            }
        }
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_filename_extension_buffer_overflow);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}