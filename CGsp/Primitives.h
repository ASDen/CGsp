typedef Modifier* pModifier;

class CGSP_CC Primitives
{
public:
	std::vector<pModifier> ModStack;
	Polyhedron Mesh;
	Polyhedron ModifiedMesh;

	Point_3* Center;

	virtual Polyhedron Draw()=0;

	inline void setMesh(Polyhedron& P)
	{
		Mesh=P;
		ModifiedMesh=P;
	}

	void ApplyModifier(pModifier M)
	{
		ModStack.push_back(M);
	}

	void UpdateAtFrame(int Fnum)
	{
		std::vector<pModifier>::iterator i;
		ModifiedMesh=Mesh;
		for(i=ModStack.begin();i!=ModStack.end();i++)
		{
			(*i)->DoAtFrame(ModifiedMesh,Fnum);
		}
	}

	void maxmin(int &value, int min, int max)
	{
		if (value < min)
			value = min;
		else if (value > max)
			value = max;
	}

	void min(double &value, double min)
	{
		if (value < min)
			value = min;
	}
	
	Point_3* get_ModifiedMeshCenter()
	{
		double x = 0, y = 0, z = 0;
		int num = 0;
		for (Vertex_iterator i = ModifiedMesh.vertices_begin(); i != ModifiedMesh.vertices_end(); i++)
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

		Point_3* Center = new Point_3(x, y, z);

		return Center;
	}

	Point_3* get_MeshCenter()
	{
		double x = 0, y = 0, z = 0;
		int num = 0;
		for (Vertex_iterator i = Mesh.vertices_begin(); i != Mesh.vertices_end(); i++)
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

		Point_3* Center = new Point_3(x, y, z);

		return Center;
	}
};
/////////////
/////////////
struct XYZ {
	double x,y,z;

	void Normalise(XYZ *p)
	{
		double length;

		length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
		if (length != 0) {
			p->x /= length;
			p->y /= length;
			p->z /= length;
		} else {
			p->x = 0;
			p->y = 0;
			p->z = 0;
		}
	}
	XYZ MidPoint(XYZ p1,XYZ p2)
	{
		XYZ p;

		p.x = (p1.x + p2.x) / 2;
		p.y = (p1.y + p2.y) / 2;
		p.z = (p1.z + p2.z) / 2;

		return(p);
	}
} ;
typedef struct {
   XYZ p1,p2,p3;   
} FACET3;
////////////
