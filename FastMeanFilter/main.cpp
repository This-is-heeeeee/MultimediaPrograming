#include <opencv2\opencv.hpp>

void myFastestMeanFilter(IplImage * src, IplImage * dst, int k)
{
	CvSize size = cvGetSize(src);
	CvScalar **sum = new CvScalar*[size.height];

	for (int i = 0; i < size.height; i++) {
		sum[i] = new CvScalar[size.width];
	}

	for(int y = 0; y < size.height; y++)
		for (int x = 0; x < size.width; x++)
		{
			CvScalar s = cvGet2D(src, y, x);
			sum[y][x].val[0] = s.val[0];
			sum[y][x].val[1] = s.val[1];
			sum[y][x].val[2] = s.val[2];
		}

	for (int y = 0; y < size.height; y++)
		for (int x = 1; x < size.width; x++)
		{
			sum[y][x].val[0] += sum[y][x - 1].val[0];
			sum[y][x].val[1] += sum[y][x - 1].val[1];
			sum[y][x].val[2] += sum[y][x - 1].val[2];
		}
	for (int y = 1; y < size.height; y++)
		for (int x = 0; x < size.width; x++)
		{
			sum[y][x].val[0] += sum[y-1][x].val[0];
			sum[y][x].val[1] += sum[y-1][x].val[1];
			sum[y][x].val[2] += sum[y-1][x].val[2];
		}

	for (int y = k; y < size.height - k; y++)
		for (int x = k; x < size.width - k; x++)
		{
			CvScalar s = sum[y + k][x + k];
			
			if (y - k - 1 < 0 && x - k - 1 < 0) {
				s.val[0] /= ((2 * k + 1)*(2 * k + 1));
				s.val[1] /= ((2 * k + 1)*(2 * k + 1));
				s.val[2] /= ((2 * k + 1)*(2 * k + 1));
				cvSet2D(dst, y, x, s);
				continue;
			}//(k,k)
			else if (y - k - 1 < 0)
			{
				s.val[0] = (s.val[0] - sum[y + k][x - k - 1].val[0]) / ((2 * k + 1)*(2 * k + 1));
				s.val[1] = (s.val[1] - sum[y + k][x - k - 1].val[1]) / ((2 * k + 1)*(2 * k + 1));
				s.val[2] = (s.val[2] - sum[y + k][x - k - 1].val[2]) / ((2 * k + 1)*(2 * k + 1));
				cvSet2D(dst, y, x, s);
				continue;
			}//(x,k)
			else if (y - k - 1 < 0)
			{
				s.val[0] = (s.val[0] - sum[y - k - 1][x + k].val[0]) / ((2 * k + 1)*(2 * k + 1));
				s.val[1] = (s.val[1] - sum[y - k - 1][x + k].val[1]) / ((2 * k + 1)*(2 * k + 1));
				s.val[2] = (s.val[2] - sum[y - k - 1][x + k].val[2]) / ((2 * k + 1)*(2 * k + 1));
				cvSet2D(dst, y, x, s);
				continue;
			}//(k,y)
			s.val[0] = (s.val[0] - sum[y - k - 1][x + k].val[0] - sum[y + k][x - k - 1].val[0] + sum[y - k - 1][x - k - 1].val[0])/ ((2 * k + 1)*(2 * k + 1));
			s.val[1] = (s.val[1] - sum[y - k - 1][x + k].val[1] - sum[y + k][x - k - 1].val[1] + sum[y - k - 1][x - k - 1].val[1])/ ((2 * k + 1)*(2 * k + 1));
			s.val[2] = (s.val[2] - sum[y - k - 1][x + k].val[2] - sum[y + k][x - k - 1].val[2] + sum[y - k - 1][x - k - 1].val[2])/ ((2 * k + 1)*(2 * k + 1));
																																							
			cvSet2D(dst, y, x, s);
		}

	for (int i = 0; i < size.height; i++)
		delete[] sum[i];
	delete[] sum;
}

int main()
{
	char filepath[100];
	IplImage *src;

	printf("=================================================\n");
	printf("Dept. of Digital Contents, Sejong University\n");
	printf("Multimedia Programming Class\n");
	printf("Homework #3: Fast Mean Filter\n");
	printf("=================================================\n");
	while (true)
	{
		printf("Input File Path : ");
		scanf("%s", filepath);
		src = cvLoadImage(filepath);
		if (src != NULL) break;
		printf("Wrong Path!\n");
	}

	CvSize size = cvGetSize(src);
	IplImage *dst;
	int k;
	while (true)
	{
		 dst = cvCreateImage(size, 8, 3);

		printf("Input Kernel Size K : ");
		scanf("%d", &k);
		if (k <= 0) break;
		else if (2 * k + 1 >= size.height || 2 * k + 1 >= size.width)
		{
			printf("Wrong Number!\n");
			continue;
		}

		myFastestMeanFilter(src, dst, k);
		cvShowImage("src", src);
		cvShowImage("dst", dst);
		cvMoveWindow("src", 300, 100);
		cvMoveWindow("dst", 750, 100);
		cvWaitKey(1);
	
	}

}