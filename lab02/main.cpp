
#include "pch.h"
#include "GeometricTransformer.h"

int main(int argc, char* argv[])
{
	int result = 1;
	/*if (argc < 3)
	{
		printf("Sai dong lenh\n");
		printf("<Program.exe> <Command> <InputPath> <CmdArguments>");
		return result;
	}

	string inputPath = argv[2];
	int kWidth = 3;
	int kHeight = 3;
	Mat sourceImage = imread(inputPath, CV_LOAD_IMAGE_GRAYSCALE);
	Mat destinationImage;

	if (argc == 5)
	{
		kWidth = cStringToInt(argv[3]);
		kHeight = cStringToInt(argv[4]);
	}


	if (string(argv[1]) == "--mean")
	{
		Blur blur;
		result = blur.BlurImage(sourceImage, destinationImage, kWidth, kHeight, LOC_TRUNG_BINH);
	}

	else if (string(argv[1]) == "--median")
	{
		Blur blur;
		result = blur.BlurImage(sourceImage, destinationImage, kWidth, kHeight, LOC_TRUNG_VI);
	}

	else if (string(argv[1]) == "--gauss")
	{
		Blur blur;
		result = blur.BlurImage(sourceImage, destinationImage, kWidth, kHeight, LOC_GAUSS);
	}

	else if (string(argv[1]) == "--sobel")
	{
		EdgeDetector edgeDetector;
		result = edgeDetector.DetectEdge(sourceImage, destinationImage, kWidth, kHeight, CANH_SOBEL);
	}

	else if (string(argv[1]) == "--prewitt")
	{
		EdgeDetector edgeDetector;
		result = edgeDetector.DetectEdge(sourceImage, destinationImage, kWidth, kHeight, CANH_PREWITT);
	}

	else if (string(argv[1]) == "--laplace")
	{
		EdgeDetector edgeDetector;
		result = edgeDetector.DetectEdge(sourceImage, destinationImage, kWidth, kHeight, CANH_LAPACE);
	}

	else
	{
		printf("Sai dong lenh\n");
		printf("<Program.exe> <Command> <InputPath> <CmdArguments>");
	}

	if (result == 0)
	{
		cv::imshow("Source Image", sourceImage);
		cv::imshow("Destination Image", destinationImage);
		waitKey(0);
	}
	else
	{
		printf("Errors");
	}*/

	return result;
}