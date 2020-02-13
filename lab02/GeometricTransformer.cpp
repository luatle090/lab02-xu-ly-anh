#include "GeometricTransformer.h"


Mat AffineTransform::multipleMatrix(Mat matrix1, Mat matrix2)
{
	Mat result(matrix1.rows, matrix1.cols, CV_32FC1, Scalar(0));

	int widthStep = matrix2.step[0] / matrix2.step[1];
	int nChannel = result.channels();
	
	for (int row = 0; row < matrix1.rows; row++)
	{
		float* pRowResult = result.ptr<float>(row);
		for (int col = 0; col < matrix1.cols; col++, pRowResult += nChannel)
		{
			float* pRowMatrix1 = matrix1.ptr<float>(row);
			float* pRowMatrix2 = matrix2.ptr<float>(0) + col;

			for (int k = 0; k < matrix1.cols; pRowMatrix1 += nChannel, pRowMatrix2 += widthStep)
			{
				pRowResult[0] += pRowMatrix1[0] * pRowMatrix2[0];
			}
		}
	}
	return result;
}

Mat AffineTransform::buildTranslate(float dx, float dy)
{
	Mat matrix = Mat(3, 3, CV_32FC1, Scalar(0));
	matrix.at<float>(0, 0) = 1;
	matrix.at<float>(1, 1) = 1;
	matrix.at<float>(2, 2) = 1;
	return matrix;
}

Mat AffineTransform::buildScale(float sx, float sy)
{
	Mat matrix = Mat(3, 3, CV_32FC1, Scalar(0));
	matrix.at<float>(2, 2) = 1;
	matrix.at<float>(0, 0) = sx;
	matrix.at<float>(1, 1) = sy;
	return matrix;
}

Mat AffineTransform::buildRotate(float angle)
{
	Mat matrix = Mat(3, 3, CV_32FC1, Scalar(0));
	matrix.at<float>(2, 2) = 1;
	matrix.at<float>(0, 0) = cos(angle);
	matrix.at<float>(0, 1) = -sin(angle);
	matrix.at<float>(1, 0) = sin(angle);
	matrix.at<float>(1, 1) = cos(angle);
	return matrix;
}

void AffineTransform::Translate(float dx, float dy)
{
	if (_matrixTransform.data == NULL)
	{
		_matrixTransform = buildTranslate(dx, dy);
	}
	else
	{
		Mat matrix = buildTranslate(dx, dy);
		_matrixTransform = multipleMatrix(_matrixTransform, matrix);
	}
}

void AffineTransform::Rotate(float angle)
{
	if (_matrixTransform.data == NULL)
	{
		_matrixTransform = buildRotate(angle);
	}
	else
	{
		Mat matrix = buildRotate(angle);
		_matrixTransform = multipleMatrix(_matrixTransform, matrix);
	}
}

void AffineTransform::Scale(float sx, float sy)
{
	if (_matrixTransform.data == NULL)
	{
		_matrixTransform = buildScale(sx, sy);
	}
	else
	{
		Mat matrix = buildScale(sx, sy);
		_matrixTransform = multipleMatrix(_matrixTransform, matrix);
	}
}

void AffineTransform::TransformPoint(float & x, float & y)
{
	//vector nhân ma trận
	x = x * _matrixTransform.at<float>(0, 0) + x * _matrixTransform.at<float>(0,1) + _matrixTransform.at<float>(0, 2);
	y = y * _matrixTransform.at<float>(0, 0) + y * _matrixTransform.at<float>(1, 1) + _matrixTransform.at<float>(1, 2);
}

AffineTransform::AffineTransform()
{
	
}

AffineTransform::~AffineTransform()
{
}

uchar NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
{
	return uchar();
}



int GeometricTransformer::Transform(const Mat & beforeImage, Mat & afterImage, AffineTransform * transformer, PixelInterpolate * interpolator)
{
	
	if (beforeImage.cols <= 0 || beforeImage.rows <= 0 || beforeImage.data == NULL)
		return 0;

	float x, y;
	transformer->TransformPoint(x, y);
	//interpolator->Interpolate();




	

	return 1;
}
