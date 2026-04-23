#include"../twogd.h"

world::world() 
{

}

world::world(CODEC3D* o_Codec_)
{
	o_Layers = NULL;
	o_Codec = o_Codec_;
}

uint32_t world::AppendLayer(LPSTR c_StreamName, COLOR c_Color)
{
	if (o_Layers == NULL)
	{
		o_Layers = (LAYER*)malloc(sizeof(LAYER));
		if (o_Layers == NULL) return NULL;

		o_Layers[0] = LAYER(c_StreamName, c_Color);
		i_Length = 1;
		return 0;
	}

	LAYER* o_pTemp = o_Layers;

	o_Layers = (LAYER*)realloc(o_Layers, (i_Length + 1) * sizeof(LAYER));
	if (o_Layers == NULL)
	{
		o_Layers = o_pTemp;
		return NULL;
	}

	o_Layers[i_Length] = LAYER(c_StreamName, c_Color);
	i_Length++;
	return i_Length - 1;
}

uint8_t world::Render()
{
	for (uint32_t i_Index = 0; i_Index < i_Length; i_Index++)
	{
		o_Codec->DrawObject(&o_Layers[i_Index].o_Obj, &o_Layers[i_Index].c_Color, o_Layers[i_Index].i_PixelFlag, o_Layers[i_Index].i_PrioFlag);
	}

	return GD_TASK_OKAY;
}

uint8_t world::GetObjInCrosshair(CAM3D* o_pCam, FLOAT f_CutoffRadius, uint32_t* i_pLayer)
{
	for (uint32_t i_Layer = 0; i_Layer < i_Length; i_Layer++)
	{
		V2 v_ScreenPos;
		if (o_Codec->o_Camera->Translate(&o_Layers[i_Layer].o_Obj.v_Anchor, &v_ScreenPos) != GD_TASK_OKAY) continue;

		FLOAT f_Dist = (FLOAT)v_ScreenPos.Distance(V2(o_Codec->o_Camera->i_Dimensions[0] / 2.0f, o_Codec->o_Camera->i_Dimensions[1] / 2.0f));
		if (f_Dist < f_CutoffRadius)
		{
			*i_pLayer = i_Layer;
			return GD_TASK_OKAY;
		}
	}
	*i_pLayer = i_Length + 1;
	return GD_OUTOFBOUND;
}

uint8_t world::Dispose()
{
	if (o_Layers != NULL)
	{
		for (uint32_t i_index = 0; i_index < i_Length; i_index++)
		{
			o_Layers[i_index].o_Obj.Dispose();
		}
		free(o_Layers);
	}
	return GD_TASK_OKAY;
}