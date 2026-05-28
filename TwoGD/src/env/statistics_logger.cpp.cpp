#include "../twogd.h"

statlog::statlog()
{

}


statlog::statlog(PERLOG* o_pPerlog_, FONTHANDLER* o_pFont_, CAM3D* o_pCam_)
{
	o_pPerlog = o_pPerlog_;
	o_pFont = o_pFont_;
	o_pCam = o_pCam_;
}

void statlog::PrintStats()
{

	if (o_pPerlog == NULL || o_pFont == NULL || o_pCam == NULL) 
	{
		return;
	}
	COLOR* o_pColor = (COLOR*)&co_White;
	if (o_pPerlog->GetDelta() > 40.0)o_pColor = (COLOR*)&co_Red;
	o_pFont->Write(V2(10, 30), 25, o_pColor, "%.0fFPS @ %.1fms\n", 1.0f / (o_pPerlog->GetDelta() / 1000.0f), o_pPerlog->GetDelta());
	o_pFont->Write(V2(10, 90), 25, (COLOR*)&co_White,"P %.1f Y %.1f\n", RADTODEG(o_pCam->i_Rotation.f_Pos[X]), RADTODEG(o_pCam->i_Rotation.f_Pos[Y]));
	int32_t i_LastCursor = o_pFont->Write(V2(10, 60), 25, (COLOR*)&co_Red, "X %.1f \n", o_pCam->i_Position.f_Pos[X]);
	i_LastCursor = o_pFont->Write(i_LastCursor, V2(10, 60), 25, (COLOR*)&co_Green, "Y %.1f \n", o_pCam->i_Position.f_Pos[Y]);
	o_pFont->Write(i_LastCursor, V2(10, 60), 25, (COLOR*)&co_Blue,"Z %.1f\n", o_pCam->i_Position.f_Pos[Z]);
}