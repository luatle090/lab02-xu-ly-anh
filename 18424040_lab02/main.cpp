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



	Mat sourceImage = imread("E:\\lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat dstImage, dstImage2;
	GeometricTransformer geo;
	PixelInterpolate *pixel;
	NearestNeighborInterpolate langGieng;
	BilinearInterpolate songTuyenTinh;
	pixel = &langGieng;
	//geo.RotateUnkeepImage(sourceImage, dstImage, -30, pixel);
	//geo.Flip(sourceImage, dstImage, 0, pixel);
	//geo.Scale(sourceImage, dstImage, 4, 4, pixel);

	pixel = &songTuyenTinh;
	geo.Scale(sourceImage, dstImage2, 4, 4, pixel);
	//geo.RotateKeepImage(sourceImage, dstImage2, -32, pixel);


	//for (int y = 0; y < sourceImage.rows; y++)
	//{
	//	uchar* pRow = sourceImage.ptr<uchar>(y);
	//	for (int x = 0; x < sourceImage.cols; x++, pRow += sourceImage.channels())
	//	{
	//		uchar s = sourceImage.at<uchar>(y, x);//truy xuất pixel (x,y)
	//		uchar c =pRow[0];
	//		//pRow[1];
	//		//pRow[2];
	//		float sum = 0;
	//	}
	//}


	cv::imshow("Source Image", sourceImage);
	//cv::imshow("Destination Image", dstImage);
	cv::imshow("Destination Image 2", dstImage2);
	waitKey(0);
	return result;
}