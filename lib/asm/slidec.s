/******************************
 *     ＳＬＩ展開ルーチン     *
 *         1995/04/05         *
 * Programmed By Melody-Yoshi *
 ******************************/
	.align	4

	.text
	.globl	slidec
	.ent	slidec
	.set    reorder
/* ====== はじまるよん ====== [IN:R4=DATA OUT:R5=bz] */
slidec:		lw	$24,8($4)	## R24=出力サイズ
		lw	$25,12($4)	## R25=PBUF offset
		move	$6,$0		## flags=0
		add	$4,16
		add	$24,$5
		add	$25,$4		## R25=PBUF アドレス
/*	Reg
##	R04:圧縮データのアドレス
##	R05:出力データのアドレス
##	R06:読み込みのカウンタ
##	R08:
##	R09:
##	R10:
##	R11:
##	R24:出力サイズ
##	R25:ｐｂｕｆアドレス
##	R26-System Reserved
*/
/* ====== ＳＬＩ展開メイン ====== */
slidemain:	bne	$6,$0,codecheck
		lh	$8,($4)
		li	$6,16
		add	$4,2
		sll	$8,$6
codecheck:	slt	$9,$8,$0	## MSBチェック
		beq	$9,$0,pressdata
		lb	$10,($25)
		add	$25,1
		sb	$10,($5)
		add	$5,1
		b	loopend
pressdata:	lhu	$10,($4)
		add	$4,2
		srl	$11,$10,12
		and	$10,0xfff
		sub	$9,$5,$10
		add	$11,3
pressloop:	lb	$10,-1($9)
		sub	$11,1
		add	$9,1
		sb	$10,($5)
		add	$5,1
		bne	$11,$0,pressloop
loopend:	sll	$8,1
		sub	$6,1
		blt	$5,$24,slidemain
		jr	$31
	.end slidec
