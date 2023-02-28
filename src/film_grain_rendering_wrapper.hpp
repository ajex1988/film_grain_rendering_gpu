//
// Created by zhe.zhu on 2/25/2023.
//

#ifndef FILM_GRAIN_RENDERING_GPU_FILM_GRAIN_RENDERING_WRAPPER_H
#define FILM_GRAIN_RENDERING_GPU_FILM_GRAIN_RENDERING_WRAPPER_H

#define MAX_CHANNELS 3

#ifdef _MSC_VER
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYMBOL float*  fg_render(float* pImg, int h, int w, int nc, float grainSize, int colorActivated, int nIter);

EXPORT_SYMBOL void  free_mem(float* p);
#ifdef __cplusplus
}
#endif

#endif //FILM_GRAIN_RENDERING_GPU_FILM_GRAIN_RENDERING_WRAPPER_H
