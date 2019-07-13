#include <iostream> 
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <Eigen/Dense>
#include "string"
#include "cxxopts.hpp"
#include "io.hpp"

using namespace Eigen;
//void convertMat2UC(MatrixXf gray,unsigned char *& gray_UC,int size);

int main(int argc, char* argv[] ){
	//Nota bene: casting big images to unsigned char in Eigen result in a segmentation fault. Compiler complains that the array is too big. We have therefore chosen the following way to convert Eigen matrix to unsigned char *

	cxxopts::Options options("MyProgram", "One line description of MyProgram");
	options.add_options()
		("i,image_path", "Path to binary input image", cxxopts::value<std::string>()) 
		("o,output_path", "Path to save result", cxxopts::value<std::string>()) ;
	auto result = options.parse(argc, argv);

	std::string filename = result["image_path"].as<std::string>(); 
	std::string output_filename  = result["output_path"].as<std::string>(); 
	std::cout<<filename<<std::endl;

	//Load image
	int x,y,n;
 	unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 0);
	if (data==NULL){
		std::cout<< "Cant load image"<<std::endl;
	}

	//Part array to Eigen matrix
	const int nx = x;
	const int ny = y;
	std::cout<<x<<" "<<y<<" "<<n<<std::endl;
	typedef Matrix<unsigned char,Dynamic, Dynamic> MatrixXUC;
	typedef Map<MatrixXUC,RowMajor> MapType;
	MapType img(data,y,x);
	//Map<Matrix<unsigned char,y,x>,RowMajor > img(data,y,x ); 
	MatrixXf gray = img.cast<float>();
	//Convert back to raw buffer 
	unsigned char * gray_UC; 
	convertMat2UC(gray,gray_UC,x*y); 
	stbi_write_jpg(output_filename.c_str(),x,y,n,gray_UC,100); 

	return 0;
}
#endif
#endif
//void convertMat2UC(MatrixXf gray,unsigned char * &gray_UC,int size)
//{
//	unsigned char gray_UC_tmp[size];
//	float *gray_array = gray.data();
//	for (int i=0; i<size;++i){
//		gray_UC_tmp[i] = static_cast<unsigned char> (gray_array[i]);	
//	}
//	gray_UC = gray_UC_tmp;
//}
