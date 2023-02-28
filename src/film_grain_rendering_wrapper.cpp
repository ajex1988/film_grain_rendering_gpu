//
// Created by zhe.zhu on 2/27/2023.
//
#include "film_grain_rendering.h"
#include "film_grain_rendering_wrapper.hpp"
#include "matrix.h"

#include <iostream>
#include <string>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <algorithm>

float* fg_render(float* pImg, int imgH, int imgW, int nC, float grainSize, int colorActivated, int nIter) {

    float sigmaR, s, sigmaFilter, xA, yA, xB, yB;
    int mOut, nOut;
    unsigned int randomizeSeed;

    /**************************************************/
    /*************   READ INPUT IMAGE   ***************/
    /**************************************************/
    float *imgInFloat;
    size_t widthIn, heightIn, nChannels;

    imgInFloat = pImg;
    widthIn = imgW;
    heightIn = imgH;
    nChannels = nC;

    /**************************************************/
    /*************   SET INPUT OPTIONS   **************/
    /**************************************************/
    sigmaR = 0.0;
    s = 1.0;
    sigmaFilter = 0.8;
    xA = 0;
    yA = 0;
    xB = widthIn;
    yB = heightIn;

    mOut = (int) heightIn;
    nOut = (int) widthIn;
    randomizeSeed = 1;

    if (nChannels == 1)
        colorActivated = 0;

    //create film grain options structure
    filmGrainOptionsStruct<float> filmGrainParams;

    filmGrainParams.muR = grainSize;
    filmGrainParams.sigmaR = sigmaR;
    filmGrainParams.s = 1.0;    //zoom
    filmGrainParams.sigmaFilter = (float) sigmaFilter;
    filmGrainParams.NmonteCarlo = nIter;    //number of monte carlo iterations
    filmGrainParams.xA = xA;
    filmGrainParams.yA = yA;
    filmGrainParams.xB = xB;
    filmGrainParams.yB = yB;
    filmGrainParams.mOut = mOut;
    filmGrainParams.nOut = nOut;
    filmGrainParams.randomizeSeed = randomizeSeed;


    //display parameters
    std::cout << "Input image size : " << widthIn << " x " << heightIn << std::endl;
    std::cout << "grainRadius : " << filmGrainParams.muR << std::endl;
    std::cout << "sigmaR : " << filmGrainParams.sigmaR << std::endl;
    std::cout << "sigmaFilter : " << filmGrainParams.sigmaFilter << std::endl;
    std::cout << "NmonteCarlo : " << filmGrainParams.NmonteCarlo << std::endl;
    if (colorActivated == 0)
        std::cout << "black and white" << std::endl;
    else
        std::cout << "colour" << std::endl;
    std::cout << "randomizeSeed : " << filmGrainParams.randomizeSeed << std::endl;


    /**************************************************/
    /*****  TIME AND CARRY OUT GRAIN RENDERING   ******/
    /**************************************************/

    struct timeval start, end;
    gettimeofday(&start, NULL);

    //create output float image
    float *imgOut = new float[(mOut) * (nOut) * ((unsigned int) MAX_CHANNELS)];

    matrix<float> *imgIn = new matrix<float>();
    imgIn->allocate_memory((int) heightIn, (int) widthIn);
    //create pseudo-random number generator for the colour seeding

    //execute the film grain synthesis
    std::cout << "***************************" << std::endl;
    for (unsigned int colourChannel = 0; colourChannel < ((unsigned int) MAX_CHANNELS); colourChannel++) {
        float *imgOutTemp;
        //copy memory
        for (unsigned int i = 0; i < (unsigned int) heightIn; i++) {
            for (unsigned int j = 0; j < (unsigned int) widthIn; j++) {
                imgIn->set_value(i, j, (float) imgInFloat[(int) i * ((unsigned int) widthIn) + (int) j +
                                                          colourChannel * ((unsigned int) widthIn) *
                                                          ((unsigned int) heightIn)]);
            }
        }
        //normalise input image
        imgIn->divide((float) (MAX_GREY_LEVEL + EPSILON_GREY_LEVEL)); // RS

        /***************************************/
        /**   carry out film grain synthesis  **/
        /***************************************/
        imgOutTemp = film_grain_rendering_pixel_wise_cuda(imgIn->get_ptr(),
                                                          imgIn->get_ncols(), imgIn->get_nrows(), nOut, mOut,
                                                          filmGrainParams);

        std::cout<<"BP 1"<<std::endl;
        //put the output image back to [0, 255]
        for (int i = 0; i < (mOut * nOut); i++)
            imgOutTemp[i] = imgOutTemp[i] * ((float) (MAX_GREY_LEVEL + EPSILON_GREY_LEVEL));
        std::cout<<"BP 2"<<std::endl;
        if (colorActivated > 0)    //colour grain
        {
            std::cout<<"BP 3"<<std::endl;
            for (unsigned int i = 0; i < (unsigned int) mOut; i++)
                for (unsigned int j = 0; j < ((unsigned int) nOut); j++)
                    imgOut[j + i * (nOut) +
                           colourChannel * (mOut) * (nOut)] =
                            imgOutTemp[j + i * (nOut)];
            delete imgOutTemp;
        } else    //black-and-white
        {
            std::cout<<"BP 4"<<std::endl;
            for (unsigned int i = 0; i < (unsigned int) mOut; i++)
                for (unsigned int j = 0; j < (unsigned int) nOut; j++) {
                    imgOut[j + i * (nOut)] =
                            imgOutTemp[j + i * (nOut)];        //red
                    imgOut[j + i * (nOut) + (mOut) * (nOut)] =
                            imgOutTemp[j + i * (nOut)];    //green
                    imgOut[j + i * (nOut) + 2 * (mOut) * (nOut)] =
                            imgOutTemp[j + i * (nOut)];    //blue
                }
            delete imgOutTemp;
            break;
        }
    }
    std::cout<<"BP 5"<<std::endl;
//    write_output_image(imgOut, fileNameOut, filmGrainParams, (unsigned int) MAX_CHANNELS);

//    delete imgInFloat;
    delete imgIn;
//    delete imgOut;

    gettimeofday(&end, NULL);
    double elapsedTime = (end.tv_sec - start.tv_sec) +
                         (end.tv_usec - start.tv_usec) / 1.e6;
    std::cout << "time elapsed : " << elapsedTime << std::endl;
    std::cout << "***************************" << std::endl << std::endl << std::endl;


    return imgOut;
}

void free_mem(float * p) {
    delete p;
}
