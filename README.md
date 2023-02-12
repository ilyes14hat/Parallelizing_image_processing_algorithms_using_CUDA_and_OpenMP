# Parallizing_image_processing_algorithms_using_CUDA_and_OpenMP
</br>

## Sequential algorithm:
* The code for the sequential image processing algorithms is found in the sequential folder.
* To run the sequential functions:

  1. Clone the project.
  2. The functions only work with pgm P2 images (there is a sample in the Sequential/images folder)
  3. If you want to add another image put it in the same folder and rename it irv.pgm
  4. Run the command ./Sequential/function_name (for example ./Sequential/invert
  5. The resulting image will be stored in irv_out.pgm (You can view pgm images directly if you are on linux or use a pgm viewer if on windows)

## Parallel OpenMP algorithm:
* The code for the Parallel image processing algorithms using OpenMP is found in the OpenMP folder.
* To run the parallel functions:

  1. Clone the project.
  2. The functions only work with pgm P2 images (there is a sample in the Parallel/images folder)
  3. If you want to add another image put it in the same folder and rename it irv.pgm
  4. Run the command ./Parallel/function_name (for example ./Parallel/invert
  5. The resulting image will be stored in irv_out.pgm (You can view pgm images directly if you are on linux or use a pgm viewer if on windows)
  
## Parallel CUDA algorithm:
* The code for the Parallel image processing algorithms using CUDA is found in the CUDA folder.
* To run the Parallel functions:

  1. Clone the project.
  2. Open the notebook in google colab.
  3. Change the runtime to use GPU.
  4. Run the initialization cells.
  5. Upload the irv.pgm file found in either Sequential/images or OpenMP/images
  6. Run any function (in the success cells the function are succefully parallized but the failure ones aren't but still gives correct results)
  7. The resulting image will be stored in irv_out.pgm 
  8. Download it(You can view pgm images directly if you are on linux or use a pgm viewer if on windows)
  
## Results:
* The results (comparaison) of the 3 algorithms can be found in the results folder.
* Download the notebook and open it.
* The results are saved in the output (You can rerun it if you want but you must run the graph functions first.
