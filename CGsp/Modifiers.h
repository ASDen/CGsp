class Modifier
{
public:
	typedef std::vector<AnimatableProperyParent*> propsVector;

	propsVector props;

	virtual void Do(Polyhedron&) = 0;

	void DoAtFrame(Polyhedron& P,int Frame)
	{
		propsVector::iterator i;
		for(i=props.begin();i!=props.end();i++)
		{
			(*i)->SetAtFrame(Frame);
		}
		Do(P);
	}

	inline Aff3 Cgal2Eign_TM (Eigen::Transform3d& T)
	{
		return Aff3( T(0,0), T(0,1), T(0,2), T(0,3),
					 T(1,0), T(1,1), T(1,2), T(1,3),
					 T(2,0), T(2,1), T(2,2), T(2,3));
	}

	Eigen::Vector3d CalcFaceOrigin (Facet_iterator F_iter)
	{
		Point_3 point;
		Eigen::Vector3d Original(0,0,0);
		Halfedge_facet_circulator j = F_iter->facet_begin();

		do 
		{
			point = j->vertex()->point();
			Original += Eigen::Vector3d (point.x(), point.y(), point.z());
		} while ( ++j != F_iter->facet_begin());

		Original /= CGAL::circulator_size (j);
		return Original;
	}

	void ApplyTransformOnFace(Aff3 aff,Facet_iterator F_iter)
	{
		Point_3 point;
		Halfedge_facet_circulator j = F_iter->facet_begin();

		do 
		{
			point = j->vertex()->point();
			point = point.transform(aff);
			j->vertex()->point() = point;
		} while ( ++j != F_iter->facet_begin());
	}

	Aff3 CalcExecInFaceCoordSys(Eigen::Transform3d& E_Trans,Facet_iterator F_iter,Plane_const_iterator P_iter,Point_3 point)
	{
		Eigen::Transform3d T;
		Eigen::Quaterniond Q;
		Eigen::Vector3d Normal (P_iter->x(), P_iter->y(), P_iter->z());
		Eigen::Vector3d plp (point.x(), point.y(), point.z());
		Eigen::Vector3d Original = CalcFaceOrigin(F_iter);

		Normal.normalize();
		plp -= Original;
		double Dist = Normal.dot(plp);
		
		Q = Q.setFromTwoVectors (Normal, Eigen::Vector3d(0, 0, 1));
		Q.normalize();

		T.setIdentity();
		T.pretranslate (-Original);
		T.prerotate (Q.toRotationMatrix());
		T.pretranslate (Eigen::Vector3d(0, 0, -Dist));
		T = E_Trans * T;
		T.pretranslate (Eigen::Vector3d(0, 0, Dist));
		T.prerotate (Q.conjugate().toRotationMatrix());
		T.pretranslate (Original);

		return Cgal2Eign_TM(T);
	}

	void ApplyTransformToPolyhedron(Polyhedron &P,Eigen::Transform3d& E_Trans)
	{
		Aff3 aff = Cgal2Eign_TM(E_Trans);
		for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); ++i)
		{
			i->point() = i->point().transform(aff);
		}
	}

	Point_3 calc_Center(Polyhedron P)
	{
		double x = 0, y = 0, z = 0;
		int num = 0;
		for (Vertex_iterator i = P.vertices_begin(); i != P.vertices_end(); i++)
		{
			Point_3 p = i->point();
			num++;

			x += p.x();
			y += p.y();
			z += p.z();
		}
		x /= num;
		y /= num;
		z /= num;

		Point_3 Center = Point_3(x, y, z);

		return Center;
	}
};

typedef enum
{
	X_ax,Y_ax,Z_ax
}Axis;

typedef enum
{
	BRadial,BLinear
}BulgeType;

//typedef enum
//{
//	Group,Loacl_Normal,Polygon
//}Type;