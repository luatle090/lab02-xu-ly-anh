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
	x = x * _matrixTransform.at<float>(0, 0) + x * _matrixTransform.at<float>(0, 1) + _matrixTransform.at<float>(0, 2);
	y = y * _matrixTransform.at<float>(1, 0) + y * _matrixTransform.at<float>(1, 1) + _matrixTransform.at<float>(1, 2);
}

AffineTransform::AffineTransform()
{
	
}

AffineTransform::~AffineTransform()
{
}


uchar* NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
{
	
	uchar result = 0;
	int cols = tx;
	int rows = ty;
	pSrc = pSrc + rows * srcWidthStep + cols * nChannels;

	//if (tx - (int)tx == 0 && ty - (int)ty == 0)
	//{
	//	result = pSrc[0];
	//}

	return pSrc;
}

NearestNeighborInterpolate::NearestNeighborInterpolate()
{
}

NearestNeighborInterpolate::~NearestNeighborInterpolate()
{
}


uchar *BilinearInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
{
	return uchar();
}

BilinearInterpolate::BilinearInterpolate()
{
}

BilinearInterpolate::~BilinearInterpolate()
{
}


int GeometricTransformer::Transform(const Mat & beforeImage, Mat & afterImage, AffineTransform * transformer, PixelInterpolate * interpolator)
{
	
	if (afterImage.cols <= 0 || afterImage.rows <= 0 || afterImage.data == NULL)
		return 0;

	int rows = afterImage.rows;
	int cols = afterImage.cols;

	int srcWidthStep = beforeImage.step[0];
	int srcChannels = beforeImage.step[1];

	int dstWidthStep = afterImage.step[0];
	int dstChannels = afterImage.step[1];

	uchar* pDstData = (uchar*)afterImage.data;
	uchar* pSrc = (uchar*)beforeImage.data;

	for (int row = 0; row < rows; row++, pDstData += dstWidthStep)
	{
		uchar* pRow = pDstData;
		for (int col = 0; col < cols; col++, pRow += dstChannels)
		{
			float x = col, y = row;
			transformer->TransformPoint(x, y);

			//nếu x, y là điểm pixel trong ảnh gốc thì giá trị là nó
			//ngược lại thì giá trị sẽ đc nội suy
			uchar* p = interpolator->Interpolate(x, y, pSrc, srcWidthStep, srcChannels);
			
			
			pRow[0] = p[0];
			pRow[1] = p[1];
			pRow[2] = p[2];
		}
	}
	return 1;
}

int GeometricTransformer::Scale(const Mat & srcImage, Mat & dstImage, float sx, float sy, PixelInterpolate * interpolator)
{
	int result = 0;
	int width = srcImage.cols;
	int height = srcImage.rows;

	dstImage = Mat(height * sy, width * sx, CV_8UC3, Scalar(0));

	AffineTransform affine;
	affine.Scale(1.0/sx, 1.0/sy);

	result = Transform(srcImage, dstImage, &affine, interpolator);
	return result;
}

int GeometricTransformer::Resize(const Mat & srcImage, Mat & dstImage, int newWidth, int newHeight, PixelInterpolate * interpolator)
{
	int result = 0;

	if (srcImage.cols <= 0 || srcImage.rows <= 0 || srcImage.data == NULL)
		return 0;

	int width = srcImage.cols;
	int height = srcImage.rows;

	dstImage = Mat(newHeight, newWidth, CV_8UC3, Scalar(0));

	float x = 1.0 / (1.0 * newWidth / width);
	float y = 1.0 / (1.0 * newHeight / height);

	AffineTransform affine;
	affine.Scale(x, y);

	result = Transform(srcImage, dstImage, &affine, interpolator);
	return result;
}

GeometricTransformer::GeometricTransformer()
{
}

GeometricTransformer::~GeometricTransformer()
{
}



PixelInterpolate::PixelInterpolate()
{
}

PixelInterpolate::~PixelInterpolate()
{
}
