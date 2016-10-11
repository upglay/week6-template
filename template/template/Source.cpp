#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

const int width = 640;
const int height = 480;

float* pixels = new float[width * height * 3];

class Tcircle
{
public:
	void draw(const int& center_x_, const int& center_y_)
	{
		float radius_ = 75.0f;
		float thickness_ = 3;

		float theta = 2 * 3.1415926 / 1000.0f;
		float c = cosf(theta);//precalculate the sine and cosine
		float s = sinf(theta);
		float t;

		float x = radius_;
		float y = 0;

		for (int i = 0; i < thickness_; i++)
		{
			x = radius_ + i;
			y = 0;
			for (int ii = 0; ii < 1000; ii++)
			{
				pixels[((int)(x + center_x_) + width*(int)(y + center_y_)) * 3 + 0] = 1.0f;	//red
				pixels[((int)(x + center_x_) + width*(int)(y + center_y_)) * 3 + 1] = 0.0f;	//green
				pixels[((int)(x + center_x_) + width*(int)(y + center_y_)) * 3 + 2] = 0.0f;	//blue

																							//apply the rotation matrix
				t = x;
				x = c * x - s * y;
				y = s * t + c * y;
			}
		}
	}
};


class Tbox
{
public:
	void draw(const int& center_x_, const int& center_y_)
	{
		float radius_ = 150.0f;

		drawLine(center_x_ - (radius_ / 2), center_y_ + (radius_ / 2), center_x_ + (radius_ / 2), center_y_ + (radius_ / 2));
		drawLine(center_x_ + (radius_ / 2), center_y_ + (radius_ / 2), center_x_ + (radius_ / 2), center_y_ - (radius_ / 2));
		drawLine(center_x_ + (radius_ / 2), center_y_ - (radius_ / 2), center_x_ - (radius_ / 2), center_y_ - (radius_ / 2));
		drawLine(center_x_ - (radius_ / 2), center_y_ - (radius_ / 2), center_x_ - (radius_ / 2), center_y_ + (radius_ / 2));
	}

	void drawLine(const int& i0, const int& j0, const int& i1, const int& j1)
	{
		if (i0 < i1)
		{
			for (int i = i0; i <= i1; i++)
			{
				const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

				pixels[(i + width*j) * 3 + 0] = 1.0;	//red
				pixels[(i + width*j) * 3 + 1] = 0.0f;	//green
				pixels[(i + width*j) * 3 + 2] = 0.0f;	//blue
			}
		}
		else if (i0 > i1)
		{
			for (int i = i1; i <= i0; i++)
			{
				const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

				pixels[(i + width*j) * 3 + 0] = 1.0;	//red
				pixels[(i + width*j) * 3 + 1] = 0.0f;	//green
				pixels[(i + width*j) * 3 + 2] = 0.0f;	//blue
			}
		}
		else
		{
			if (j0 < j1)
			{
				for (int j = j0; j < j1; j++)
				{
					pixels[(i0 + width*j) * 3 + 0] = 1.0;	//red
					pixels[(i0 + width*j) * 3 + 1] = 0.0f;	//green
					pixels[(i0 + width*j) * 3 + 2] = 0.0f;	//blue
				}
			}
			else if (j0 > j1)
			{
				for (int j = j1; j < j0; j++)
				{
					pixels[(i0 + width*j) * 3 + 0] = 1.0f;	//red
					pixels[(i0 + width*j) * 3 + 1] = 0.0f;	//green
					pixels[(i0 + width*j) * 3 + 2] = 0.0f;	//blue
				}
			}
			else
			{
				pixels[(i0 + width*j0) * 3 + 0] = 1.0f;	//red
				pixels[(i0 + width*j0) * 3 + 1] = 0.0f;	//green
				pixels[(i0 + width*j0) * 3 + 2] = 0.0f;	//blue
			}
		}
	}
};


class GeometricObjectInterface
{
public:
	virtual void draw(const int& x, const int& y) = 0;
};

template<class TT>
class GeometricObject : public GeometricObjectInterface
{
public:
	void draw(const int& x, const int& y)
	{
		TT operation;
		operation.draw(x, y);
	}
};



int main(void)
{
	GLFWwindow* window;


	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			pixels[(i + width*j) * 3 + 0] = 1.0f;	//red
			pixels[(i + width*j) * 3 + 1] = 1.0f;	//green
			pixels[(i + width*j) * 3 + 2] = 1.0f;	//blue
		}

	std::vector<GeometricObjectInterface*> obj_list;
	obj_list.push_back(new GeometricObject<Tcircle>);
	obj_list.push_back(new GeometricObject<Tbox>);
		
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//geo_vector[0]->clearBackgroundColor(pixels);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//draw circle and square
		int i = 0;
		for (auto itr : obj_list)
		{
			itr->draw(200 + 200 * i, 200);
			i++;
		}

		glDrawPixels(640, 480, GL_RGB, GL_FLOAT, pixels);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

