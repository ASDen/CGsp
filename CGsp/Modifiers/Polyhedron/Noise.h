#include "RandomNoise.h"

class Noise : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> Amplitude;
	AnimatablePropery<double,Interpolator> Frequency;
	AnimatablePropery<double,Interpolator> Offset;
	Point_3* Center;
	Axis RoAxis;

	AnimatablePropery<double,Interpolator> X_Center;
	AnimatablePropery<double,Interpolator> Y_Center;
	AnimatablePropery<double,Interpolator> Z_Center;

	Noise() : Amplitude(0),Frequency(10),Offset(0),Center(NULL),RoAxis(Z_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp) : Amplitude(Amp),Frequency(10),Offset(0),Center(NULL),RoAxis(Z_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, double wavlen) : Amplitude(Amp),Frequency(wavlen),Offset(0),Center(NULL),RoAxis(Z_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, Point_3* C) : Amplitude(Amp),Frequency(10),Offset(0),Center(C),RoAxis(Z_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}
	
	Noise(double Amp, Axis RAxis) : Amplitude(Amp),Frequency(10),Offset(0),Center(NULL),RoAxis(),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, double wavlen, double pha) : Amplitude(Amp),Frequency(wavlen),Offset(pha),Center(NULL),RoAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, double wavlen, Point_3* C) : Amplitude(Amp),Frequency(wavlen),Offset(0),Center(C),RoAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, double wavlen, Axis RAxis) : Amplitude(Amp),Frequency(wavlen),Offset(0),Center(NULL),RoAxis(RAxis),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, Point_3* C, Axis RAxis) : Amplitude(Amp),Frequency(10),Offset(0),Center(C),RoAxis(RAxis),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, double wavlen, double pha, Point_3* C) : Amplitude(Amp),Frequency(wavlen),Offset(pha),Center(C),RoAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, double wavlen, double pha, Axis RAxis) : Amplitude(Amp),Frequency(wavlen),Offset(pha),Center(NULL),RoAxis(RAxis),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Noise(double Amp, double wavlen, double pha, Point_3* C, Axis RAxis) : Amplitude(Amp),Frequency(wavlen),Offset(pha),Center(C),RoAxis(RAxis),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Frequency);
		props.push_back(&Offset);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	void Do(Polyhedron &P)
	{
		if (Frequency.val == 0 && Amplitude.val == 0)
			return;

		Eigen::Transform3d t;
		Eigen::Vector3d org;

		double x, y, z;
		double Scale;

		Vertex_iterator Begin = P.vertices_begin();
		
		/*if (Center == NULL)
		{
			Center = &calc_Center(P);

			X_Center = Center->x();
			Y_Center = Center->y();
			Z_Center = Center->z();
		}*/

		double x_c = X_Center.val;
		double y_c = Y_Center.val;
		double z_c = Z_Center.val;

		t.setIdentity();
		org=Eigen::Vector3d(x_c,y_c,z_c);
		t.pretranslate(-org);
		ApplyTransformToPolyhedron(P,t);

		switch (RoAxis)
		{
		case X_ax:
			{
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					Point_3 point = Point_3(0, Offset.val + Frequency.val * p.y(), Offset.val + Frequency.val * p.z());
					Point_3 noise = Randomize::noise<Point_3>(point);

					Scale =  Amplitude.val * 2.0 * (noise[0] - 0.5);

					x = p.x() + Scale;
					y = p.y();
					z = p.z();
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Y_ax:
			{
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					Point_3 point = Point_3(Offset.val + Frequency.val * p.x(), 0, Offset.val + Frequency.val * p.z());
					Point_3 noise = Randomize::noise<Point_3>(point);

					Scale =  Amplitude.val * 2.0 * (noise[1] - 0.5);

					x = p.x();
					y = p.y() + Scale;
					z = p.z();
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Z_ax:
			{
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					Point_3 point = Point_3(Offset.val + Frequency.val * p.x(), Offset.val + Frequency.val * p.y(), 0);
					Point_3 noise = Randomize::noise<Point_3>(point);

					Scale =  Amplitude.val * 2.0 * (noise[2] - 0.5);

					x = p.x();
					y = p.y();
					z = p.z() + Scale;
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		default:
			{
				return;
			}
		}

		t.setIdentity();
		t.pretranslate(org);
		ApplyTransformToPolyhedron(P,t);
	}
};