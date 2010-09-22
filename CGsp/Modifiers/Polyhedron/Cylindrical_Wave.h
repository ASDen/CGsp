class CGSP_CC Cylindrical_Wave : public Modifier
{
public:
	AnimatablePropery<double,Interpolator> Amplitude;
	AnimatablePropery<double,Interpolator> Wavelength;
	AnimatablePropery<double,Interpolator> Phase;
	Point_3* Center;
	Axis AfAxis;

	AnimatablePropery<double,Interpolator> X_Center;
	AnimatablePropery<double,Interpolator> Y_Center;
	AnimatablePropery<double,Interpolator> Z_Center;

	Cylindrical_Wave() : Amplitude(0),Wavelength(10),Phase(0),Center(NULL),AfAxis(Z_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp) : Amplitude(Amp),Wavelength(10),Phase(0),Center(NULL),AfAxis(Z_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, double wavlen) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(NULL),AfAxis(Z_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, Point_3* C) : Amplitude(Amp),Wavelength(10),Phase(0),Center(C),AfAxis(Z_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}
	
	Cylindrical_Wave(double Amp, Axis afAxis) : Amplitude(Amp),Wavelength(10),Phase(0),Center(NULL),AfAxis(afAxis),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, double wavlen, double pha) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(NULL),AfAxis(X_ax),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, double wavlen, Point_3* C) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(C),AfAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, double wavlen, Axis afAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(0),Center(NULL),AfAxis(afAxis),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, Point_3* C, Axis afAxis) : Amplitude(Amp),Wavelength(10),Phase(0),Center(C),AfAxis(afAxis),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, double wavlen, double pha, Point_3* C) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(C),AfAxis(X_ax),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, double wavlen, double pha, Axis afAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(NULL),AfAxis(afAxis),X_Center(0),Y_Center(0),Z_Center(0)
	{
		props.push_back(&Amplitude);
		props.push_back(&Wavelength);
		props.push_back(&Phase);
		props.push_back(&X_Center);
		props.push_back(&Y_Center);
		props.push_back(&Z_Center);
	}

	Cylindrical_Wave(double Amp, double wavlen, double pha, Point_3* C, Axis afAxis) : Amplitude(Amp),Wavelength(wavlen),Phase(pha),Center(C),AfAxis(afAxis),X_Center(C->x()),Y_Center(C->y()),Z_Center(C->z())
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

		double x, y, z;
		double delta, Scale;

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

		switch (AfAxis)
		{
		case X_ax:
			{
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					delta = Phase.val + (2 * CGAL_PI * p.x() / Wavelength.val);
					Scale = Amplitude.val * sin(delta);

					double Rad = sqrt( pow( p.y(), 2) + pow( p.z(), 2));
					Rad = Rad == 0 ? 1 : Rad;

					x = p.x();
					y = p.y() + Scale * p.y() / Rad;
					z = p.z() + Scale * p.z() / Rad;
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Y_ax:
			{
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					delta = Phase.val + (2 * CGAL_PI * p.y() / Wavelength.val);
					Scale = Amplitude.val * sin(delta);

					double Rad = sqrt( pow( p.x(), 2) + pow( p.z(), 2));
					Rad = Rad == 0 ? 1 : Rad;


					x = p.x() + Scale * p.x() / Rad;
					y = p.y();
					z = p.z() + Scale * p.z() / Rad;
					
					i->point() = Point_3 (x ,y ,z);
				}
				break;
			}

		case Z_ax:
			{				
				for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i) 
				{
					Point_3 p = i->point();

					delta = Phase.val + (2 * CGAL_PI * p.z() / Wavelength.val);
					Scale = Amplitude.val * sin(delta);

					double Rad = sqrt( pow( p.x(), 2) + pow( p.y(), 2));
					Rad = Rad == 0 ? 1 : Rad;

					x = p.x() + Scale * p.x() / Rad;
					y = p.y() + Scale * p.y() / Rad;
					z = p.z();
					
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