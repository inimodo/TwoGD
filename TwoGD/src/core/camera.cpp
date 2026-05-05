#include "..\twogd.h"

camera::camera()
{
	f_FOV = 2;
	f_Frustum[0] = 30;
	f_Frustum[1] = 1;
	i_Dimensions[X] = 16 * 50;
	i_Dimensions[Y] = 9 * 50;
	i_Position = V3(0, 0, 0);
	i_Rotation = V3(0, 0, 0);

	V2 v_ScreenPos = V2((float)i_Dimensions[0], (float)i_Dimensions[1]);
	V3 v_Angles = V3(0, 0, 0);
	Relate(&v_ScreenPos, &v_Angles);
	f_CutoffAngles[0] = v_Angles.f_Pos[X] + DEGTORAD(10);
	f_CutoffAngles[1] = v_Angles.f_Pos[Y] + DEGTORAD(10);
}

camera::camera(uint32_t i_Width, uint32_t i_Height)
{
	f_FOV = 2.0f;
	f_Frustum[1] = 250.0f;
	f_Frustum[0] = 0.01f;
	i_Dimensions[X] = i_Width;
	i_Dimensions[Y] = i_Height;
	i_Position = V3(0, 0, 0);
	i_Rotation = V3(0, 0, 0);

	V2 v_ScreenPos = V2((float)i_Dimensions[0], (float)i_Dimensions[1]);
	V3 v_Angles = V3(0, 0, 0);
	Relate(&v_ScreenPos, &v_Angles);
	f_CutoffAngles[0] = v_Angles.f_Pos[X] + DEGTORAD(10);
	f_CutoffAngles[1] = v_Angles.f_Pos[Y] + DEGTORAD(10);
}

uint8_t camera::Translate(V3* v_pPoint, V2* v_pResult)
{
	V3 o_Delta = *v_pPoint - i_Position;
	float f_abs = o_Delta.Length();

	if (f_abs > f_Frustum[1] || f_abs < f_Frustum[0]) return GD_OUTOFBOUND;

	o_Delta.CamRotateThisOpt(i_Rotation);

	if (o_Delta.f_Pos[2] < 0) return GD_OUTOFBOUND;

	float f_leftright = asin(o_Delta.f_Pos[0] / f_abs);
	float f_updown = asin(o_Delta.f_Pos[1] / f_abs);

	if ((b_UseCutoffAngles == TRUE) &&
		(abs(f_leftright) > f_CutoffAngles[0] || abs(f_updown) > f_CutoffAngles[1])) return GD_OUTOFBOUND;

	float f_standoff = i_Dimensions[0] / f_FOV;
	v_pResult->f_Pos[0] = tan(f_leftright) * f_standoff;
	v_pResult->f_Pos[1] = (tan(f_updown) / cos(f_leftright)) * f_standoff;

	v_pResult->f_Pos[1] *= -1; // Flip because y=0 is on top
	v_pResult->f_Pos[0] += i_Dimensions[0] / 2.0f;
	v_pResult->f_Pos[1] += i_Dimensions[1] / 2.0f;

	return GD_TASK_OKAY;
}

uint8_t camera::Relate(V2* v_pScreenPos, V3* v_pAngle)
{
	float f_standoff = i_Dimensions[0] / f_FOV;
	V2 f_Input = V2(
		(v_pScreenPos->f_Pos[0] - (i_Dimensions[0] / 2.0f)) / f_standoff,
		(v_pScreenPos->f_Pos[1] - (i_Dimensions[1] / 2.0f)) / -f_standoff
	);

	v_pAngle->f_Pos[0] = atan(f_Input.f_Pos[0]);
	v_pAngle->f_Pos[1] = -atan(f_Input.f_Pos[1] * cos(v_pAngle->f_Pos[0]));

	return GD_TASK_OKAY;
}