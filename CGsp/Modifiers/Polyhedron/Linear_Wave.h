class CGSP_CC Linear_Wave : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> Amplitude;
	AnimatablePropery<double,Interpolator> Wavelength;
	AnimatablePropery<double,Interpolator> Phase;
	Point_3* Center;
	Axis RoAxis;
	Axis AfAxis;

	AnimatablePropery<double,Interpolator> X_Center;
	AnimatablePropery<double,Interpolator> Y_Center;
	AnimatablePropery<double,Interpolator> Z_Center;

	Linear_Wave() : Amplitude(0),Wavelength(10),Phase(0),Center(NULL),RoAxis(Z_ax),AfAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp) : Amplitude(Amp),Wavelength(10),Phase(0),Center(NULL),RoAxis(Z_ax),AfAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(NULL),RoAxis(Z_ax),AfAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, Point_3* C) : Amplitude(Amp),Wavelength(10),Phase(0),Center(C),RoAxis(Z_ax),AfAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}
	
	Linear_Wave(double Amp, Axis RAxis) : Amplitude(Amp),Wavelength(10),Phase(0),Center(NULL),RoAxis(RAxis),AfAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, double pha) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(NULL),RoAxis(Z_ax),AfAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, Point_3* C) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(C),RoAxis(Z_ax),AfAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, Axis RAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(NULL),RoAxis(RAxis),AfAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, Point_3* C, Axis RAxis) : Amplitude(Amp),Wavelength(10),Phase(0),Center(C),RoAxis(RAxis),AfAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, Axis RAxis, Axis afAxis) : Amplitude(Amp),Wavelength(10),Phase(0),Center(NULL),RoAxis(RAxis),AfAxis(afAxis),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, double pha, Point_3* C) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(C),RoAxis(Z_ax),AfAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, double pha, Axis RAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(NULL),RoAxis(RAxis),AfAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}
	
	Linear_Wave(double Amp, double wavlen, Point_3* C, Axis RAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(C),RoAxis(RAxis),AfAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, Axis RAxis, Axis afAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(NULL),RoAxis(RAxis),AfAxis(afAxis),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}
		
	Linear_Wave(double Amp, Point_3* C, Axis RAxis, Axis afAxis) : Amplitude(Amp),Wavelength(10),Phase(0),Center(C),RoAxis(RAxis),AfAxis(afAxis),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, double pha, Point_3* C, Axis RAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(C),RoAxis(RAxis),AfAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, double pha, Axis RAxis, Axis afAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(NULL),RoAxis(RAxis),AfAxis(afAxis),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Linear_Wave(double Amp, double wavlen, Point_3* C, Axis RAxis, Axis afAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(C),RoAxis(RAxis),AfAxis(afAxis),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	
	Linear_Wave(double Amp, double wavlen, double pha, Point_3* C, Axis RAxis, Axis afAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(C),RoAxis(RAxis),AfAxis(afAxis),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	void Do(Polyhedron &P)
	{
		if (Wavelength.val == 0 && Amplitude.val == 0)
			return;

		Eigen::Transform3d t;
		Eigen::Vector3d org;

		double x_max = 0, y_max = 0, z_max = 0, x_min = 0, y_min = 0, z_min = 0;
		double x, y, z;
		double delta, Scale;

		Vertex_iterator Begin = P.vertices_begin();
		x_max = x_min = Begin->point().x();
		y_max = y_min = Begin->point().y();
		z_max = z_min = Begin->point().z();

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

					switch (AfAxis)
					{
					case X_ax:
						delta = Phase.val + (2 * CGAL_PI * p.x() / Wavelength.val);
						break;
					case Y_ax:
						delta = Phase.val + (2 * CGAL_PI * p.y() / Wavelength.val);
						break;
					case Z_ax:
						delta = Phase.val + (2 * CGAL_PI * p.z() / Wavelength.val);
						break;
					}

					Scale = Amplitude.val * sin(delta);

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

					switch (AfAxis)
					{
					case X_ax:
						delta = Phase.val + (2 * CGAL_PI * p.x() / Wavelength.val);
						break;
					case Y_ax:
						delta = Phase.val + (2 * CGAL_PI * p.y() / Wavelength.val);
						break;
					case Z_ax:
						delta = Phase.val + (2 * CGAL_PI * p.z() / Wavelength.val);
						break;
					}

					Scale = Amplitude.val * sin(delta);

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

					switch (AfAxis)
					{
					case X_ax:
						delta = Phase.val + (2 * CGAL_PI * p.x() / Wavelength.val);
						break;
					case Y_ax:
						delta = Phase.val + (2 * CGAL_PI * p.y() / Wavelength.val);
						break;
					case Z_ax:
						delta = Phase.val + (2 * CGAL_PI * p.z() / Wavelength.val);
						break;
					}

					Scale = Amplitude.val * sin(delta);

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