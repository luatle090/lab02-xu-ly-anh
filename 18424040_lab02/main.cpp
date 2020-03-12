#include "pch.h"
#include "GeometricTransformer.h"

int cStringToInt(char* str);

int cStringToInt(char* str)
{
	int temp = 0;
	int sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		temp *= 10;
		temp += (int)(*str - '0');
		str++;
	}
	return temp * sign;
}


int main(int argc, char* argv[])
{
	int result = 0;
	if (argc < 5)
	{
		printf("Sai dong lenh\n");
		printf("<Program.exe> <Command> <Interpolate> <InputPath> <CmdArguments>\n");
		return result;
	}

	string command = argv[1];
	string interpolate = argv[2];
	string inputPath = argv[3];


	Mat sourceImage = imread(inputPath, CV_LOAD_IMAGE_COLOR);
	Mat destinationImage;
	PixelInterpolate *pixel = NULL;

	//check dòng lệnh interpolate
	if (interpolate == "--bl") 
	{	
		//BilinearInterpolate songTuyenTinh;
		pixel = new BilinearInterpolate();
	}
	else if (interpolate == "--nn")
	{
		//NearestNeighborInterpolate langGieng;
		pixel = new NearestNeighborInterpolate();
	}

	else
	{
		printf("Sai dong lenh\n");
		printf("<Interpolate>: --bl, --nn\n");
	}

	
	GeometricTransformer geo;

	//check dòng lệnh command
	if (command == "--zoom")
	{
		if (argc < 6)
		{
			printf("Khong xac dinh duoc he so Y\n");
		}
		else
		{
			float heSoX = stof(argv[4]);
			float heSoY = stof(argv[5]);
			result = geo.Scale(sourceImage, destinationImage, heSoX, heSoY, pixel);
		}
	}

	else if (command == "--resize")
	{
		if (argc < 6)
		{
			printf("Khong xac dinh duoc new height\n");
		}
		else 
		{
			int newWidth = cStringToInt(argv[4]);
			int newHeight = cStringToInt(argv[5]);
			result = geo.Resize(sourceImage, destinationImage, newWidth, newHeight, pixel);
		}
	}

	else if (command == "--rotate")
	{
		float heSo = stof(argv[4]);
		result = geo.RotateKeepImage(sourceImage, destinationImage, heSo, pixel);
	}

	else if (command == "--rotateN")
	{
		float heSo = stof(argv[4]);
		result = geo.RotateUnkeepImage(sourceImage, destinationImage, heSo, pixel);
	}

	else if (command == "--flip")
	{
		int heSo = cStringToInt(argv[4]);
		result = geo.Flip(sourceImage, destinationImage, heSo, pixel);
	}

	else
	{
		printf("Sai dong lenh\n");
		printf("<Command>: --resize, --rotateN, --flip, --rotate, --zoom\n");
	}

	if (result)
	{
		cv::imshow("Source Image", sourceImage);
		cv::imshow("Destination Image", destinationImage);
		waitKey(0);
	}
	else
	{
		printf("Errors");
	}

	if(pixel != NULL)
		delete pixel;
	return result;
}