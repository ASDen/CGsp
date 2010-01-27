class Interpolator
{
public:

	template<class Type,class Interp>
	static void Interpolate( AnimatablePropery<Type,Interp>& prop)
	{
		//allocate in vector
		prop.FrameValues.assign(prop.NumberOfFrames,Type());
		int sz=prop.KeyFrames.size();
		//double *x=new double(sz);
		//double *y=new double(sz);
		double x[1000],y[1000];

		sz=0;
		for(std::map<int,Type>::iterator i=prop.KeyFrames.begin();i!=prop.KeyFrames.end();i++)
		{
			std::cout<<sz<<" "<<i->second<<std::endl;
			x[sz]=i->first;
			y[sz++]=i->second;
			//std::fill(prop.FrameValues.begin()+i->first,prop.FrameValues.end(),i->second);
		}
		//Interpolate
		{
			gsl_interp_accel *accel_ptr=gsl_interp_accel_alloc ();
			gsl_spline *spline_ptr=gsl_spline_alloc (gsl_interp_linear, sz);
			gsl_spline_init (spline_ptr, x, y, sz);

			for(int i=0;i< prop.NumberOfFrames ; i++)
			{
				prop.FrameValues[i]=(Type)gsl_spline_eval (spline_ptr, (double)i, accel_ptr);
				std::cout<<i<<"  "<<prop.FrameValues[i]<<std::endl;
			}

			//free memory
			//delete[sz] x;
			//delete[] y;
			gsl_spline_free (spline_ptr);
			gsl_interp_accel_free (accel_ptr);
		}
	}
};

class LinearInterpolator
{
	/*template<class Type>
	static void Interpolate( double x[],double y[],std::vector<Type> frames)
	{
		gsl_interp_accel *accel_ptr=gsl_interp_accel_alloc ();
		gsl_spline *spline_ptr=gsl_spline_alloc (gsl_interp_cspline, frames.size());
		gsl_spline_init (spline_ptr, x, y, frames.size());
	}*/
};

class ConstantInterpolator
{
public:
	/*template<class Type>
	static void Interpolate( AnimatablePropery<Type,ConstantInterpolator>& prop)
	{
		std::map<int,Type>::iterator i;
		prop.FrameValues.assign(prop.NumberOfFrames,Type());
		for(i=prop.KeyFrames.begin();i!=prop.KeyFrames.end();i++)
		{
			std::fill(prop.FrameValues.begin()+i->first,prop.FrameValues.end(),i->second);
		}
	}*/
};