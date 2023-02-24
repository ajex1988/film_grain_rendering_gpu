//
// Created by zhe.zhu on 2/23/2023.
//

#ifndef FILM_GRAIN_RENDERING_GPU_FILM_GRAIN_RENDERING_MAIN_H
#define FILM_GRAIN_RENDERING_GPU_FILM_GRAIN_RENDERING_MAIN_H

#include <vector>
#include <iostream>

#ifdef _MSC_VER
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYMBOL int  film_grain_rendering_gpu(const char* fIn, const char* fOut, float grainSize, int nIter);

#ifdef __cplusplus
}
#endif


#endif //FILM_GRAIN_RENDERING_GPU_FILM_GRAIN_RENDERING_MAIN_H
