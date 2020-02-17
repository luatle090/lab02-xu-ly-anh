#include "GeometricTransformer.h"
#include "pch.h"

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

			for (int k = 0; k < matrix1.cols; k++, pRowMatrix1 += nChannel, pRowMatrix2 += widthStep)
			{
				pRowResult[0] += pRowMatrix1[0] * pRowMatrix2[0];
			}
			printf("%f ", pRowResult[0]);
		}
		printf("\n");
	}
	return result;
}

Mat AffineTransform::buildTranslate(float dx, float dy)
{
	Mat matrix = Mat(3, 3, CV_32FC1, Scalar(0));
	matrix.at<float>(0, 0) = 1;
	matrix.at<float>(1, 1) = 1;
	matrix.at<float>(2, 2) = 1;
	matrix.at<float>(0, 2) = dx;
	matrix.at<float>(1, 2) = dy;
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
	float deg = M_PI / 180.0;
	Mat matrix = Mat(3, 3, CV_32FC1, Scalar(0));
	matrix.at<float>(2, 2) = 1;
	matrix.at<float>(0, 0) = cos(angle * deg);
	matrix.at<float>(0, 1) = -sin(angle * deg);
	matrix.at<float>(1, 0) = sin(angle * deg);
	matrix.at<float>(1, 1) = cos(angle * deg);
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
	float tempX = x, tempY = y;
	x = tempX * _matrixTransform.at<float>(0, 0) + tempY * _matrixTransform.at<float>(0, 1) + _matrixTransform.at<float>(0, 2);
	y = tempX * _matrixTransform.at<float>(1, 0) + tempY * _matrixTransform.at<float>(1, 1) + _matrixTransform.at<float>(1, 2);


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


uchar* BilinearInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
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
	uchar* p;
	for (int row = 0; row < rows; row++, pDstData += dstWidthStep)
	{
		uchar* pRow = pDstData;
		for (int col = 0; col < cols; col++, pRow += dstChannels)
		{
			float x = col, y = row;
			transformer->TransformPoint(x, y);
			
			if (x < 0 || x >= beforeImage.cols || y < 0 || y >= beforeImage.rows)
			{
				pRow[0] = 0;
				pRow[1] = 0;
				pRow[2] = 0;
			}
			else
			{
				//nếu x, y là điểm pixel trong ảnh gốc thì giá trị là nó
				//ngược lại thì giá trị sẽ đc nội suy
				p = interpolator->Interpolate(x, y, pSrc, srcWidthStep, srcChannels);


				pRow[0] = p[0];
				pRow[1] = p[1];
				pRow[2] = p[2];
			}	
		}
	}
	return 1;
}

int GeometricTransformer::RotateKeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	int result = 0;
	float deg = M_PI / 180.0;
	int newRows, newCols;
	AffineTransform affine;

	//xoay ngược chiều kim đồng hồ
	if (angle < 0)
	{
		//tìm góc alpha theo chiều ngược kim đồng hồ
		float angleAlpha = 90 - abs(angle);
		float dy = srcImage.cols * cos(abs(angleAlpha) * deg);
		float dx = srcImage.cols * sin(abs(angleAlpha) * deg);

		//lấy affine ngược
		affine.Rotate(-angle);
		affine.Translate(0, -dy);

		//tìm full rows và full cols;
		//tìm góc đối của angleAlpha
		float angle_2 = 90 - abs(angleAlpha);

		//tính new width
		//tìm góc bên phải
		float angle_3 = 180 - 90 - angle_2;
		newCols = dx + srcImage.rows * cos(angle_3 * deg);

		//tìm góc để tính new height
		//tìm góc bên dưới
		float angle_4 = 90 - (90 - abs(angle));
		newRows = dy + srcImage.rows * cos(angle_4 * deg);
	}
	else
	{
		float angle_2 = 180 - 90 - abs(angle);
		float angleAlpha = 90 - angle_2;

		float dy = srcImage.rows * cos(abs(angleAlpha) * deg);
		float dx = srcImage.rows * sin(abs(angleAlpha) * deg);

		//lấy affine ngược
		affine.Rotate(-angle);
		affine.Translate(-dx, 0);

		//tìm full rows và full cols;
		//tính new width
		newCols = dx + srcImage.cols * cos(angle * deg);

		//tính new height
		//tìm góc bên dưới
		float angle_4 = 180 - 90 - angleAlpha;
		newRows = dy + srcImage.cols * cos(angle_4 * deg);
	}

	dstImage = Mat(newRows + 1, newCols + 1, CV_8UC3, Scalar(0));
	result = Transform(srcImage, dstImage, &affine, interpolator);
	return result;
}

int GeometricTransformer::RotateUnkeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	int result = 0;
	float deg = M_PI / 180.0;
	int newRows, newCols;
	AffineTransform affine;

	//xoay ngược chiều kim đồng hồ
	if (angle < 0)
	{
		//tìm góc alpha theo chiều ngược kim đồng hồ
		float angleAlpha = 90 - abs(angle);
		float dy = srcImage.cols * cos(abs(angleAlpha) * deg);
		float dx = srcImage.cols * sin(abs(angleAlpha) * deg);


		//lấy affine ngược
		affine.Rotate(-angle);
		affine.Translate(0, -dy);
	}
	else
	{
		float angle_2 = 180 - 90 - abs(angle);
		float angleAlpha = 90 - angle_2;

		float dy = srcImage.rows * cos(abs(angleAlpha) * deg);
		float dx = srcImage.rows * sin(abs(angleAlpha) * deg);

		//lấy affine ngược
		affine.Rotate(-angle);
		affine.Translate(-dx, 0);
	}

	dstImage = Mat(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(0));
	result = Transform(srcImage, dstImage, &affine, interpolator);
	return result;
}

int GeometricTransformer::Scale(const Mat & srcImage, Mat & dstImage, float sx, float sy, PixelInterpolate * interpolator)
{
	int result = 0;
	int width = srcImage.cols;
	int height = srcImage.rows;

	dstImage = Mat(height * sy, width * sx, CV_8UC3, Scalar(0));

	//lấy affine ngược
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

	//lấy affine ngược
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
