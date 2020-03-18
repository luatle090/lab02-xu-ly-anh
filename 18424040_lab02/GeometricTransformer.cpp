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
	//nếu x, y là điểm pixel trong ảnh gốc thì giá trị là nó
	//ngược lại thì giá trị sẽ đc nội suy
	uchar result = 0;
	int col = tx;
	int row = ty;
	pSrc = pSrc + row * srcWidthStep + col * nChannels;
	
	return pSrc;
}

NearestNeighborInterpolate::NearestNeighborInterpolate()
{
}

NearestNeighborInterpolate::~NearestNeighborInterpolate()
{
}


void BilinearInterpolate::lookup()
{

}

uchar* BilinearInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
{
	//coding theo công thức wikipedia
	uchar value[3];
	uchar valueR1[3];
	uchar valueR2[3];

	int col = tx;
	int row = ty;
	pSrc = pSrc + row * srcWidthStep + col * nChannels;

	float phanDuX = tx - (int)tx;
	float phanDuY = ty - (int)ty;

	//giá trị tại tọa độ góc ko cần nội suy
	if (phanDuX == 0 && phanDuY == 0)
	{
		return pSrc;
	}
	else 
	{
		//nội suy giá trị R1 và R2 trước
		//sau đó tìm giá trị P(x, y) tức là giá trị cần nội suy

		uchar* Q11 = pSrc;
		uchar* Q21 = pSrc + nChannels;
		uchar* Q12 = pSrc + srcWidthStep;
		uchar* Q22 = pSrc + srcWidthStep + nChannels;

		float mau = col + 1 - col;
		float factor1, factor2;

		if (mau == 0)
		{
			factor1 = 1;
			factor2 = 0;
		}
		else
		{
			factor1 = (col + 1 - tx) / mau;
			factor2 = (tx - col) / mau;
		}

		//tính giá trị R1 và R2
		for (int k = 0; k < nChannels; k++)
		{
			valueR1[k] = factor1 * Q11[k] + factor2 * Q21[k];
			valueR2[k] = factor1 * Q12[k] + factor2 * Q22[k];
		}

		float factor3, factor4;
		if (row + 1 - row == 0)
		{
			factor3 = 1;
			factor4 = 0;
		}
		else 
		{
			factor3 = (row + 1 - ty) / (row + 1 - row);
			factor4 = (ty - row) / (row + 1 - row);
		}

		//tính giá trị cần nội suy
		for (int k = 0; k < nChannels; k++)
		{
			value[k] = factor3 * valueR1[k] + factor4 * valueR2[k];
		}
	}

	return &value[0];
}

BilinearInterpolate::BilinearInterpolate()
{
}

BilinearInterpolate::~BilinearInterpolate()
{
}


int GeometricTransformer::Transform(const Mat & beforeImage, Mat & afterImage, AffineTransform * transformer, PixelInterpolate * interpolator)
{
	
	if (afterImage.cols <= 0 || afterImage.rows <= 0 || afterImage.data == NULL || interpolator == NULL)
		return 0;

	int rows = afterImage.rows;
	int cols = afterImage.cols;

	int srcWidthStep = beforeImage.step[0];
	int srcChannels = beforeImage.step[1];

	int dstWidthStep = afterImage.step[0];
	int dstChannels = afterImage.step[1];

	uchar* pDstData = (uchar*)afterImage.data;
	uchar* pSrc = (uchar*)beforeImage.data;
	uchar* pRGB;
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
				if (x >= beforeImage.cols - 1)
					x = x - 1;
				if (y >= beforeImage.rows - 1)
					y = y - 1;
				pRGB = interpolator->Interpolate(x, y, pSrc, srcWidthStep, srcChannels);


				pRow[0] = pRGB[0];
				pRow[1] = pRGB[1];
				pRow[2] = pRGB[2];
			}	
		}
	}
	return 1;
}

int GeometricTransformer::RotateKeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	int result = 0;
	if (srcImage.cols <= 0 || srcImage.rows <= 0 || srcImage.data == NULL || 
		interpolator == NULL || angle > 360 || angle < 0)
		return result;

	float deg = M_PI / 180.0;
	int newRows, newCols;
	AffineTransform affine;

	//xoay ngược chiều kim đồng hồ
	//1
	angle = -angle;
	if (angle >= -90) 
	{
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
		float angle_4 = 90 - (90 + angle);
		newRows = dy + srcImage.rows * cos(angle_4 * deg);
	}
		
	//2
	else if (angle >= -180)
	{
		float angle_6 = abs(angle) - 90;
		float angle_5 = 90 - angle_6;
		float angle_4 = 90 - abs(angle_5);
		float angleAlpha = 90 - angle_4;

		float dy = srcImage.cols * sin(abs(angle_5) * deg);
		float dx = srcImage.cols * cos(angle_5 * deg);

		//tìm full rows và full cols;
		//tính new width
		newCols = dx + srcImage.rows * abs(cos(angle_6 * deg));

		//tính new height
		newRows = dy + srcImage.rows * cos(angleAlpha * deg);

		dy = newRows;

		//lấy affine ngược
		affine.Rotate(-angle);
		affine.Translate(-dx, -dy);
	}

	//3
	else if (angle >= -270)
	{ 
		float angle_8 = abs(angle) - 180;
		float angle_7 = 90 - angle_8;
		float angle_3 = 90 - angle_8;
		float angle_2 = 90 - angle_3;

		float dy = srcImage.rows * cos(angle_8 * deg);
		float dx = srcImage.rows * sin(angle_8 * deg);

		//tìm full rows và full cols;
		//tính new width
		newCols = dx + srcImage.cols * abs(cos(angle_2 * deg));

		//tính new height
		newRows = dy + srcImage.cols * cos(angle_7 * deg);

		dx = newCols;

		//lấy affine ngược
		affine.Rotate(-angle);
		affine.Translate(-dx, -dy);
	}

	//4
	else if(angle >= -360)
	{
		float angle_3 = 360 + angle;

		float angle_2 = 90 - angle_3;
		float angleAlpha = 90 - angle_2;

		float dy = srcImage.rows * cos(abs(angleAlpha) * deg);
		float dx = srcImage.rows * sin(abs(angleAlpha) * deg);

		//lấy affine ngược
		affine.Rotate(-angle);
		affine.Translate(-dx, 0);

		//tìm full rows và full cols;
		//tính new width
		newCols = dx + srcImage.cols * abs(cos(angle_3 * deg));

		//tính new height
		//tìm góc bên dưới
		float angle_4 = 180 - 90 - angleAlpha;
		newRows = dy + srcImage.cols * cos(angle_4 * deg);
	}
	//}
	//else
	//{
	//	float angle_2 = 180 - 90 - abs(angle);
	//	float angleAlpha = 90 - angle_2;

	//	float dy = srcImage.rows * cos(abs(angleAlpha) * deg);
	//	float dx = srcImage.rows * sin(abs(angleAlpha) * deg);

	//	//lấy affine ngược
	//	affine.Rotate(-angle);
	//	affine.Translate(-dx, 0);

	//	//tìm full rows và full cols;
	//	//tính new width
	//	newCols = dx + srcImage.cols * abs(cos(angle * deg));

	//	//tính new height
	//	//tìm góc bên dưới
	//	float angle_4 = 180 - 90 - angleAlpha;
	//	newRows = dy + srcImage.cols * cos(angle_4 * deg);
	//}

	dstImage = Mat(newRows + 1, newCols + 1, CV_8UC3, Scalar(0));
	result = Transform(srcImage, dstImage, &affine, interpolator);
	return result;
}

int GeometricTransformer::RotateUnkeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	int result = 0;
	if (srcImage.cols <= 0 || srcImage.rows <= 0 || srcImage.data == NULL || interpolator == NULL)
		return result;

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
	if (srcImage.cols <= 0 || srcImage.rows <= 0 || sx < 0 || sy < 0 || srcImage.data == NULL || interpolator == NULL)
		return result;

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

	if (srcImage.cols <= 0 || srcImage.rows <= 0 || srcImage.data == NULL || newWidth <= 0 || newHeight <= 0 || interpolator == NULL)
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

int GeometricTransformer::Flip(const Mat & srcImage, Mat & dstImage, bool direction, PixelInterpolate * interpolator)
{
	int result = 0;

	if (srcImage.cols <= 0 || srcImage.rows <= 0 || srcImage.data == NULL || interpolator == NULL)
		return result;

	int width = srcImage.cols;
	int height = srcImage.rows;

	AffineTransform affine;

	if (direction)
	{
		//giảm đi 1 do duyệt từ 0
		affine.Translate(0, height - 1);
		affine.Scale(1, -1);
	}
	else
	{
		//giảm đi 1 do duyệt từ 0
		affine.Translate(width - 1, 0);
		affine.Scale(-1, 1);
	}
	
	dstImage = Mat(height, width, CV_8UC3, Scalar(0));

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
