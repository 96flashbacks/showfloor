/********************************************************************************
						Ultra 64 MARIO Brothers

					  luigi hierarcky data module

			Copyright 1995 Nintendo co., ltd.  All rights reserved

				This module was programmed by Y.Nishida

							June 20, 1995
 ********************************************************************************/

// Separate hierarchy is necessary for the import to work

extern Gfx RCP_luigi_head1[];
extern Gfx RCP_luigi_head2[];
extern Gfx RCP_luigi_head3[];
extern Gfx RCP_luigi_head4[];
extern Gfx RCP_luigi_head5[];

// Renamed Luigi's swimming hand, it doesn't compile if it has the same name as Mario's
extern Gfx RCP_swim_hand_l_luigi[];
extern Gfx RCP_swim_hand_r_luigi[];

extern Gfx RCP_luigi_near_body[];
extern Gfx RCP_luigi_mid_body[];
extern Gfx RCP_luigi_far_body[];

extern Gfx RCP_luigi_near0[];
extern Gfx RCP_luigi_near1[];
extern Gfx RCP_luigi_near2[];
extern Gfx RCP_luigi_near3[];
extern Gfx RCP_luigi_near4[];
extern Gfx RCP_luigi_near5[];
extern Gfx RCP_luigi_near6[];
extern Gfx RCP_luigi_near7[];
extern Gfx RCP_luigi_near8[];
extern Gfx RCP_luigi_near9[];
extern Gfx RCP_luigi_near10[];
extern Gfx RCP_luigi_near11[];
extern Gfx RCP_luigi_near14[];

extern Gfx RCP_luigi_mid0[];
extern Gfx RCP_luigi_mid1[];
extern Gfx RCP_luigi_mid2[];
extern Gfx RCP_luigi_mid3[];
extern Gfx RCP_luigi_mid4[];
extern Gfx RCP_luigi_mid5[];
extern Gfx RCP_luigi_mid6[];
extern Gfx RCP_luigi_mid7[];
extern Gfx RCP_luigi_mid8[];
extern Gfx RCP_luigi_mid9[];
extern Gfx RCP_luigi_mid10[];
extern Gfx RCP_luigi_mid11[];
extern Gfx RCP_luigi_mid14[];

extern Gfx RCP_luigi_far0[];
extern Gfx RCP_luigi_far1[];
extern Gfx RCP_luigi_far2[];
extern Gfx RCP_luigi_far3[];
extern Gfx RCP_luigi_far4[];
extern Gfx RCP_luigi_far5[];
extern Gfx RCP_luigi_far6[];
extern Gfx RCP_luigi_far7[];
extern Gfx RCP_luigi_far8[];
extern Gfx RCP_luigi_far9[];
extern Gfx RCP_luigi_far10[];
extern Gfx RCP_luigi_far11[];
extern Gfx RCP_luigi_far12[];
extern Gfx RCP_luigi_far13[];
extern Gfx RCP_luigi_far14[];

/********************************************************************************/
/*	Title luigi hierarchy data.													*/
/********************************************************************************/

static Hierarchy RCP_TitleLuigiHierarchy[] = {
		hmsJoint(RM_SURF, NULL, 0, 261-189, 0)									/* chn14_2_1	*/
		hmsBegin()
			hmsJoint(RM_SURF, RCP_luigi_near14, 0, 0, 0)							/* m_waist1_3_2	*/
			hmsBegin()
				hmsJoint(RM_SURF, RCP_luigi_near_body, 33, 0, 0)					/* m_body1_2	*/
				hmsBegin()

					hmsJoint(RM_SURF, NULL, 91, 0, 0)							/* m_head2		*/
					hmsBegin()
						hmsSelect(1, CtrlMarioEye)					/* for the eye animation	*/
						hmsBegin()
							hmsGfx(RM_SURF, RCP_luigi_head1)
							hmsGfx(RM_SURF, RCP_luigi_head2)
							hmsGfx(RM_SURF, RCP_luigi_head3)
							hmsGfx(RM_SURF, RCP_luigi_head4)
							hmsGfx(RM_SURF, RCP_luigi_head5)
						hmsEnd()
					hmsEnd()

					hmsJoint(RM_SURF, NULL, 63, -10, 65)							/* chn6			*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_near2, 0, 0, 0)				/* m_larmA1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_near1, 63, 0, 0)			/* m_larmB1_2	*/
							hmsBegin()
								hmsJoint(RM_SURF, RCP_luigi_near0, 71, 0, 0)		/* m_lhamd1_2	*/
							hmsEnd()
						hmsEnd()
					hmsEnd()

					hmsJoint(RM_SURF, NULL, 60, -10, -63)						/* chn10		*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_near5, 0, 0, 0)				/* m_rarmA1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_near4, 62, 0, 0)			/* m_rarmB1_2	*/
							hmsBegin()
								hmsJoint(RM_SURF, RCP_luigi_near3, 66, 0, 0)		/* m_rhand1_2	*/
							hmsEnd()
						hmsEnd()
					hmsEnd()

				hmsEnd()

				hmsJoint(RM_SURF, NULL, -34, -9, 39)								/* chn15		*/
				hmsBegin()
					hmsJoint(RM_SURF, RCP_luigi_near11, 0, 0, 0)					/* m_llegA1_2	*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_near10, 98, 0, 0)			/* m_llegB1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_near9, 81, 0, 0)			/* m_lfoot1_2	*/
						hmsEnd()
					hmsEnd()
				hmsEnd()

				hmsJoint(RM_SURF, NULL, -34, -9, -40)							/* chn17		*/
				hmsBegin()
					hmsJoint(RM_SURF, RCP_luigi_near8, 0, 0, 0)					/* m_rlegA1_2	*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_near7, 98, 0, 0)				/* m_rlegB1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_near6, 81, 0, 0)			/* m_rfoot1_2	*/
						hmsEnd()
					hmsEnd()
				hmsEnd()

			hmsEnd()
		hmsEnd()
	hmsEnd()
	hmsReturn()
};
/********************************************************************************/
/*	Near luigi hierarchy data.													*/
/********************************************************************************/

static Hierarchy RCP_NearLuigiHierarchy[] = {
		hmsJoint(RM_SURF, NULL, 0, 261-189, 0)									/* chn14_2_1	*/
		hmsBegin()
			hmsJoint(RM_SURF, RCP_luigi_near14, 0, 0, 0)							/* m_waist1_3_2	*/
			hmsBegin()
				hmsJoint(RM_SURF, RCP_luigi_near_body, 33, 0, 0)					/* m_body1_2	*/
				hmsBegin()

					hmsJoint(RM_SURF, NULL, 91, 0, 0)							/* m_head2		*/
					hmsBegin()
						hmsCProg(1, CtrlMarioHead)
						hmsRotate(0, 0, 0)								/* for control by program	*/
						hmsBegin()
							hmsSelect(1, CtrlMarioEye)					/* for the eye animation	*/
							hmsBegin()
								hmsGfx(RM_SURF, RCP_luigi_head1)
								hmsGfx(RM_SURF, RCP_luigi_head2)
								hmsGfx(RM_SURF, RCP_luigi_head3)
								hmsGfx(RM_SURF, RCP_luigi_head4)
								hmsGfx(RM_SURF, RCP_luigi_head5)
							hmsEnd()
						hmsEnd()
					hmsEnd()

					hmsJoint(RM_SURF, NULL, 63, -10, 65)							/* chn6			*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_near2, 0, 0, 0)				/* m_larmA1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_near1, 63, 0, 0)			/* m_larmB1_2	*/
							hmsBegin()
								hmsSelect(1, CtrlMarioHand)
								hmsBegin()
									hmsJoint(RM_SURF, RCP_luigi_near0, 71, 0, 0)		/* m_lhamd1_2	*/
									hmsJoint(RM_SURF, RCP_swim_hand_l_luigi, 71, 0, 0)
								hmsEnd()
							hmsEnd()
						hmsEnd()
					hmsEnd()

					hmsJoint(RM_SURF, NULL, 60, -10, -63)						/* chn10		*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_near5, 0, 0, 0)				/* m_rarmA1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_near4, 62, 0, 0)			/* m_rarmB1_2	*/
							hmsBegin()
								hmsSelect(1, CtrlMarioHand)
								hmsBegin()
									hmsJoint(RM_SURF, RCP_luigi_near3, 66, 0, 0)		/* m_rhand1_2	*/
									hmsBegin()
										hmsSucker(0,0,0, 1,CtrlMarioTaking)
									hmsEnd()
									hmsJoint(RM_SURF, RCP_swim_hand_r_luigi, 66, 0, 0)
									hmsBegin()
										hmsSucker(0,0,0, 1,CtrlMarioTaking)
									hmsEnd()
								hmsEnd()
							hmsEnd()
						hmsEnd()
					hmsEnd()

				hmsEnd()

				hmsJoint(RM_SURF, NULL, -34, -9, 39)								/* chn15		*/
				hmsBegin()
					hmsJoint(RM_SURF, RCP_luigi_near11, 0, 0, 0)					/* m_llegA1_2	*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_near10, 98, 0, 0)			/* m_llegB1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_near9, 81, 0, 0)			/* m_lfoot1_2	*/
						hmsEnd()
					hmsEnd()
				hmsEnd()

				hmsJoint(RM_SURF, NULL, -34, -9, -40)							/* chn17		*/
				hmsBegin()
					hmsJoint(RM_SURF, RCP_luigi_near8, 0, 0, 0)					/* m_rlegA1_2	*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_near7, 98, 0, 0)				/* m_rlegB1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_near6, 81, 0, 0)			/* m_rfoot1_2	*/
						hmsEnd()
					hmsEnd()
				hmsEnd()

			hmsEnd()
		hmsEnd()
	hmsReturn()
};
/********************************************************************************/
/*	Middle luigi hierarchy data.												*/
/********************************************************************************/

static Hierarchy RCP_MidLuigiHierarchy[] = {
		hmsJoint(RM_SURF, NULL, 0, 261-189, 0)									/* chn14_2_1	*/
		hmsBegin()
			hmsJoint(RM_SURF, RCP_luigi_mid14, 0, 0, 0)							/* m_waist1_3_2	*/
			hmsBegin()
				hmsJoint(RM_SURF, RCP_luigi_mid_body, 33, 0, 0)						/* m_body1_2	*/
				hmsBegin()

					hmsJoint(RM_SURF, NULL, 91, 0, 0)							/* m_head2		*/
					hmsBegin()
						hmsCProg(1, CtrlMarioHead)
						hmsRotate(0, 0, 0)								/* for control by program	*/
						hmsBegin()
							hmsSelect(1, CtrlMarioEye)					/* for the eye animation	*/
							hmsBegin()
								hmsGfx(RM_SURF, RCP_luigi_head1)
								hmsGfx(RM_SURF, RCP_luigi_head2)
								hmsGfx(RM_SURF, RCP_luigi_head3)
								hmsGfx(RM_SURF, RCP_luigi_head4)
								hmsGfx(RM_SURF, RCP_luigi_head5)
							hmsEnd()
						hmsEnd()
					hmsEnd()

					hmsJoint(RM_SURF, NULL, 63, -10, 65)							/* chn6			*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_mid2, 0, 0, 0)				/* m_larmA1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_mid1, 63, 0, 0)			/* m_larmB1_2	*/
							hmsBegin()
								hmsSelect(1, CtrlMarioHand)
								hmsBegin()
									hmsJoint(RM_SURF, RCP_luigi_mid0 , 71, 0, 0)		/* m_lhamd1_2	*/
									hmsJoint(RM_SURF, RCP_swim_hand_l_luigi, 71, 0, 0)
								hmsEnd()
							hmsEnd()
						hmsEnd()
					hmsEnd()

					hmsJoint(RM_SURF, NULL, 60, -10, -63)						/* chn10		*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_mid5, 0, 0, 0)				/* m_rarmA1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_mid4, 62, 0, 0)			/* m_rarmB1_2	*/
							hmsBegin()
								hmsSelect(1, CtrlMarioHand)
								hmsBegin()
									hmsJoint(RM_SURF, RCP_luigi_mid3 , 66, 0, 0)		/* m_rhand1_2	*/
									hmsBegin()
										hmsSucker(0,0,0, 1,CtrlMarioTaking)
									hmsEnd()
									hmsJoint(RM_SURF, RCP_swim_hand_r_luigi, 66, 0, 0)
									hmsBegin()
										hmsSucker(0,0,0, 1,CtrlMarioTaking)
									hmsEnd()
								hmsEnd()
							hmsEnd()
						hmsEnd()
					hmsEnd()

				hmsEnd()

				hmsJoint(RM_SURF, NULL, -34, -9, 39)								/* chn15		*/
				hmsBegin()
					hmsJoint(RM_SURF, RCP_luigi_mid11, 0, 0, 0)					/* m_llegA1_2	*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_mid10, 98, 0, 0)				/* m_llegB1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_mid9, 81, 0, 0)			/* m_lfoot1_2	*/
						hmsEnd()
					hmsEnd()
				hmsEnd()

				hmsJoint(RM_SURF, NULL, -34, -9, -40)							/* chn17		*/
				hmsBegin()
					hmsJoint(RM_SURF, RCP_luigi_mid8, 0, 0, 0)					/* m_rlegA1_2	*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_mid7, 98, 0, 0)				/* m_rlegB1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_mid6, 81, 0, 0)			/* m_rfoot1_2	*/
						hmsEnd()
					hmsEnd()
				hmsEnd()

			hmsEnd()
		hmsEnd()
	hmsReturn()
};
/********************************************************************************/
/*	Far luigi hierarchy data.													*/
/********************************************************************************/

static Hierarchy RCP_FarLuigiHierarchy[] = {
		hmsJoint(RM_SURF, NULL, 0, 261-189, 0)									/* chn14_2_1	*/
		hmsBegin()
			hmsJoint(RM_SURF, RCP_luigi_far14, 0, 0, 0)							/* m_waist1_3_2	*/
			hmsBegin()
				hmsJoint(RM_SURF, RCP_luigi_far13, 33, 0, 0)						/* m_body1_2	*/
				hmsBegin()

					hmsJoint(RM_SURF, NULL, 91, 0, 0)							/* m_head2		*/
					hmsBegin()
						hmsCProg(1, CtrlMarioHead)
						hmsRotate(0, 0, 0)								/* for control by program	*/
						hmsBegin()
							hmsGfx(RM_SURF, RCP_luigi_far12)
						hmsEnd()
					hmsEnd()

					hmsJoint(RM_SURF, NULL, 63, -10, 65)							/* chn6			*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_far2, 0, 0, 0)				/* m_larmA1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_far1, 63, 0, 0)			/* m_larmB1_2	*/
							hmsBegin()
								hmsSelect(1, CtrlMarioHand)
								hmsBegin()
									hmsJoint(RM_SURF, RCP_luigi_far0 , 71, 0, 0)		/* m_lhamd1_2	*/
									hmsJoint(RM_SURF, RCP_swim_hand_l_luigi, 71, 0, 0)
								hmsEnd()
							hmsEnd()
						hmsEnd()
					hmsEnd()

					hmsJoint(RM_SURF, NULL, 60, -10, -63)						/* chn10		*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_far5, 0, 0, 0)				/* m_rarmA1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_far4, 62, 0, 0)			/* m_rarmB1_2	*/
							hmsBegin()
								hmsSelect(1, CtrlMarioHand)
								hmsBegin()
									hmsJoint(RM_SURF, RCP_luigi_far3 , 66, 0, 0)		/* m_rhand1_2	*/
									hmsBegin()
										hmsSucker(0,0,0, 1,CtrlMarioTaking)
									hmsEnd()
									hmsJoint(RM_SURF, RCP_swim_hand_r_luigi, 66, 0, 0)
									hmsBegin()
										hmsSucker(0,0,0, 1,CtrlMarioTaking)
									hmsEnd()
								hmsEnd()
							hmsEnd()
						hmsEnd()
					hmsEnd()

				hmsEnd()

				hmsJoint(RM_SURF, NULL, -34, -9, 39)								/* chn15		*/
				hmsBegin()
					hmsJoint(RM_SURF, RCP_luigi_far11, 0, 0, 0)					/* m_llegA1_2	*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_far10, 98, 0, 0)				/* m_llegB1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_far9, 81, 0, 0)			/* m_lfoot1_2	*/
						hmsEnd()
					hmsEnd()
				hmsEnd()

				hmsJoint(RM_SURF, NULL, -34, -9, -40)							/* chn17		*/
				hmsBegin()
					hmsJoint(RM_SURF, RCP_luigi_far8, 0, 0, 0)					/* m_rlegA1_2	*/
					hmsBegin()
						hmsJoint(RM_SURF, RCP_luigi_far7, 98, 0, 0)				/* m_rlegB1_2	*/
						hmsBegin()
							hmsJoint(RM_SURF, RCP_luigi_far6, 81, 0, 0)			/* m_rfoot1_2	*/
						hmsEnd()
					hmsEnd()
				hmsEnd()

			hmsEnd()
		hmsEnd()
	hmsReturn()
};

/********************************************************************************/
/*	Luigi hierarchy map data.													*/
/********************************************************************************/

Hierarchy RCP_LuigiHierarchy[] = {
 	hmsShadow(100, 180, 99)
	hmsBegin()
		hmsScale(0.25f)
		hmsBegin()
			hmsSelect(1, CtrlMarioLOD)
			hmsBegin()
				hmsCall(RCP_NearLuigiHierarchy)
				hmsGroup()
				hmsBegin()
					hmsLOD(-2048,600)
					hmsBegin()
						hmsCall(RCP_NearLuigiHierarchy)
					hmsEnd()

					hmsLOD(600,1600)
					hmsBegin()
						hmsCall(RCP_MidLuigiHierarchy)
					hmsEnd()

					hmsLOD(1600,32767)
					hmsBegin()
						hmsCall(RCP_FarLuigiHierarchy)
					hmsEnd()
				hmsEnd()
			hmsEnd()
		hmsEnd()
	hmsEnd()
	hmsExit()
};
/********************************************************************************/
/*	Lugi hierarchy map data for title.											*/
/********************************************************************************/

Hierarchy RCP_TitleLuigi[] = {
	hmsScale(0.25f)
	hmsBegin()
		hmsCall(RCP_TitleLuigiHierarchy)
	hmsEnd()
	hmsExit()
};
