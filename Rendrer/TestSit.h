void Box_Wall_Spring()
{
    Box_3* s = new Box_3(4,8,4,2,2,2);
    Cylinder_3* s4 = new Cylinder_3(6,10,2,2,15);
    Spindle_3* s5 = new Spindle_3(4,5,2,48,48,48);
    Plane_3* s2 = new Plane_3(100);
    Spring_3* s6 = new Spring_3(10,2.5,40,3,20,30);
   
    s->Draw();
    s2->Draw();
    s4->Draw();
    s5->Draw();
    s6->Draw();

    PolyhedronNode* c2 = new PolyhedronNode(s2,osg::Vec3(0,0,0));
    c2->RigidActor = XPlane::Construct(pxm->gScene,c2);
    c2->WireFrame = true;
    pman->AddPolyhedron<RigidBodyManager>(c2);

    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10-i;j++)
        {
            PolyhedronNode* c1 = new PolyhedronNode(s,osg::Vec3((14+0.5*8*i)+j*8,50,i*4+2));
            c1->RigidActor = XBox::Construct(pxm->gScene,c1);
            c1->WireFrame = true;
            c1->AntialisedLines = true;
            pman->AddPolyhedron<RigidBodyManager>(c1);
        }
    }
    PolyhedronNode* cx = new PolyhedronNode(s4,osg::Vec3(50,20,20));
    cx->RigidActor = XConvex::Construct<ConvexHullReducer>(pxm->gScene,pxm->gPhysicsSDK,cx);
    cx->WireFrame = true;
    cx->AntialisedLines = true;
    pman->AddPolyhedron<RigidBodyManager>(cx);   
   
    PolyhedronNode* c3 = new PolyhedronNode(s6,osg::Vec3(50,50,200));
    c3->RigidActor = XSpring::Construct(pxm->gScene,pxm->gPhysicsSDK,c3);
    c3->WireFrame = false;
    c3->AntialisedLines = true;
    pman->AddPolyhedron<RigidBodyManager>(c3);
   
}

void Spring_Test()
{
    Plane_3* s2 = new Plane_3(100);
    Spring_3* s6 = new Spring_3(10,2.5,40,2,20,30);
    Tube_3* s4 = new Tube_3(6,5.5,6,2,2,20);
    s2->Draw();
    s6->Draw();
    s4->Draw();

    PolyhedronNode* c2 = new PolyhedronNode(s2,osg::Vec3(0,0,0));
    c2->RigidActor = XPlane::Construct(pxm->gScene,c2);
    c2->WireFrame = true;
    pman->AddPolyhedron<RigidBodyManager>(c2);

    for(int i=0;i<20;i++)
    {
        PolyhedronNode* c4 = new PolyhedronNode(s4,osg::Vec3(60,45,70+i*20));
        c4->RigidActor = XTube::Construct(pxm->gScene,c4,10);
        NxMat33 nx33;
        nx33.rotX(90);
        nx33.rotZ(45);
        c4->RigidActor->setGlobalOrientation(nx33);
        c4->WireFrame = true;
        pman->AddPolyhedron<RigidBodyManager>(c4);
    }

    PolyhedronNode* c3 = new PolyhedronNode(s6,osg::Vec3(50,50,50));
    c3->RigidActor = XSpring::Construct(pxm->gScene,pxm->gPhysicsSDK,c3,true);
    c3->WireFrame = false;
    c3->AntialisedLines = true;
    pman->AddPolyhedron<RigidBodyManager>(c3);   

}

void Spring_Test_2()
{

    NxMaterial * mat = pxm->gScene->getMaterialFromIndex(0);
    mat->setDynamicFriction(0.001);

    Plane_3* s2 = new Plane_3(100);
    Spring_3* s6 = new Spring_3(10,2.5,40,2,20,30);
    Torus_3* s3 = new Torus_3(4,1.5);
    s2->Draw();
    s6->Draw();
    s3->Draw();

    PolyhedronNode* c2 = new PolyhedronNode(s2,osg::Vec3(0,0,0));
    c2->RigidActor = XPlane::Construct(pxm->gScene,c2);
    c2->WireFrame = true;
    pman->AddPolyhedron<RigidBodyManager>(c2);

    for(int i=0;i<20;i++)
    {
        PolyhedronNode* c1 = new PolyhedronNode(s3,osg::Vec3(60,32,120+i*6));
        c1->RigidActor = XTorus::Construct(pxm->gScene,pxm->gPhysicsSDK,c1);
        c1->WireFrame = false;
        c1->AntialisedLines = true;
        pman->AddPolyhedron<RigidBodyManager>(c1);
    }

    //NxVec3 _force(1000000*NxVec3(0, -1, 0));
    //c1->Actor->addForce(_force);

    PolyhedronNode* c3 = new PolyhedronNode(s6,osg::Vec3(50,50,50));
    c3->RigidActor = XSpring::Construct(pxm->gScene,pxm->gPhysicsSDK,c3,true);
    c3->WireFrame = false;
    c3->AntialisedLines = true;
    pman->AddPolyhedron<RigidBodyManager>(c3);

    NxMat33 nx33;
    nx33.rotX(90);
    c3->RigidActor->setGlobalOrientation(nx33);
}

void Tube_Test()
{
    Plane_3* s2 = new Plane_3(100);
    Tube_3* s4 = new Tube_3(20,19.5,25,2,2,20);
    Tube_3* s8 = new Tube_3(15,14.5,25,2,2,20);
    Tube_3* s9 = new Tube_3(10,9.5,25,2,2,20);
    Sphere_3* ss = new Sphere_3(2,8);
    Box_3* s = new Box_3(4,4,4,2,2,2);
    s2->Draw();
    s4->Draw();
    s8->Draw();
    s9->Draw();
    s->Draw();
    ss->Draw();

    PolyhedronNode* c2 = new PolyhedronNode(s2,osg::Vec3(0,0,0));
    c2->RigidActor = XPlane::Construct(pxm->gScene,c2);
    c2->WireFrame = true;
    pman->AddPolyhedron<RigidBodyManager>(c2);

    PolyhedronNode* c1 = new PolyhedronNode(s4,osg::Vec3(50,50,200));
    c1->RigidActor = XTube::Construct(pxm->gScene,c1,10,true);
    c1->WireFrame = false;
    c1->AntialisedLines = true;
    pman->AddPolyhedron<RigidBodyManager>(c1);

    PolyhedronNode* c8 = new PolyhedronNode(s8,osg::Vec3(50,50,160));
    c8->RigidActor = XTube::Construct(pxm->gScene,c8,10,true);
    c8->WireFrame = false;
    c8->AntialisedLines = true;
    pman->AddPolyhedron<RigidBodyManager>(c8);

    PolyhedronNode* c9 = new PolyhedronNode(s9,osg::Vec3(50,50,120));
    c9->RigidActor = XTube::Construct(pxm->gScene,c9,10,true);
    c9->WireFrame = false;
    c9->AntialisedLines = true;
    pman->AddPolyhedron<RigidBodyManager>(c9);

    PolyhedronNode* c3 = new PolyhedronNode(s4,osg::Vec3(40,50,45));
    c3->RigidActor = XTube::Construct(pxm->gScene,c3,10,true);
    c3->WireFrame = false;
    c3->AntialisedLines = true;
    NxMat33 nx33;
    nx33.rotZ(45);
    c3->RigidActor->setGlobalOrientation(nx33);
    pman->AddPolyhedron<RigidBodyManager>(c3);

    int lm=40/4+1;
    for(int k=0;k<5;k++)
    {
        for(int i=0;i<lm;i++)
        {
            for(int j=0;j<lm;j++)
            {
                PolyhedronNode* c5 = new PolyhedronNode(ss,osg::Vec3(40+i*4,40+j*4,205+k*4));
                c5->RigidActor = XSphere::Construct(pxm->gScene,c5);
                c5->WireFrame = true;
                c5->AntialisedLines = true;
                pman->AddPolyhedron<RigidBodyManager>(c5);
            }
        }
    }
}

void cloth_test()
{
    Plane_3* s1 = new Plane_3(10);
    Plane_3* s4 = new Plane_3(16,5);
    Plane_3* s44 = new Plane_3(16,5);
    Plane_3* s45 = new Plane_3(16,100);
    Box_3* s2 = new Box_3(6,6,6,10,10,10);
    Box_3* s3 = new Box_3(9,9,9,40,40,40);
    Sphere_3* s5 = new Sphere_3(4,70);
    Spring_3* s6 = new Spring_3(10,2.5,60,5,20,30);
    Pyramid_3* s7 = new Pyramid_3(20,20,20,4,4,4);
    Torus_3* s8 = new Torus_3(5,2,0,0,30,40);
    Tube_3* s9 = new Tube_3(7,6,15,20,20,10);

    s1->Draw();
    s2->Draw();
    s3->Draw();
    s4->Draw();
    s44->Draw();
    s45->Draw();
    s5->Draw();
    s6->Draw();
    s7->Draw();
    s8->Draw();
    s9->Draw();

	Taper Ta(2,s8->Center,Y_ax,false,20,-20);
	Ta.Do(s8->Mesh);
	s8->setMesh(s8->Mesh);

    PolyhedronNode* c1 = new PolyhedronNode(s1,osg::Vec3(0,0,0));
    c1->RigidActor = XPlane::Construct(pxm->gScene,c1);
    c1->WireFrame = true;
    pman->AddPolyhedron<RigidBodyManager>(c1);

    /*PolyhedronNode* c3 = new PolyhedronNode(s3,osg::Vec3(20,0,4.5));
    c3->RigidActor = XBox::Construct(pxm->gScene,c3);
    c3->WireFrame  = false;
    pman->AddPolyhedron<RigidBodyManager>(c3);

    PolyhedronNode* c9 = new PolyhedronNode(s5,osg::Vec3(20,0,12));
    c9->RigidActor = XSphere::Construct(pxm->gScene,c9);
    c9->WireFrame  = false;
    c9->PColor = osg::Vec3(0.2,0.6,0.8);
    pman->AddPolyhedron<RigidBodyManager>(c9);
   
    PolyhedronNode* c22 = new PolyhedronNode(s44,osg::Vec3(20,0,80));
    c22->ClothActor = XCloth::Construct(pxm->gScene,pxm->gPhysicsSDK,c22,false);
    c22->ClothActor->setThickness(0.7);
    c22->WireFrame  = false;
    c22->PColor = osg::Vec3(0.85,0,0);
    pman->AddPolyhedron<ClothManager>(c22);*/

    PolyhedronNode* c6 = new PolyhedronNode(s8,osg::Vec3(-20,0,0));
    c6->RigidActor = XTorus::Construct(pxm->gScene,pxm->gPhysicsSDK,c6);
    c6->WireFrame  = false;
    c6->PColor = osg::Vec3(0.2,0.7,.2);
    pman->AddPolyhedron<RigidBodyManager>(c6);

    PolyhedronNode* ct = new PolyhedronNode(s45,osg::Vec3(-20,-5,12));
    ct->ClothActor = XCloth::Construct(pxm->gScene,pxm->gPhysicsSDK,ct,false);
    ct->ClothActor->setThickness(0.5);
    ct->WireFrame  = false;
    ct->PColor = osg::Vec3(0.85,0,0);
    pman->AddPolyhedron<ClothManager>(ct);

    /*PolyhedronNode* c4 = new PolyhedronNode(s9,osg::Vec3(0,0,10));
    c4->RigidActor = XTube::Construct2(pxm->gScene,pxm->gPhysicsSDK,c4);
    c4->WireFrame  = false;
    c4->PColor = osg::Vec3(0.8,0.3,1.5);
    pman->AddPolyhedron<RigidBodyManager>(c4);

    PolyhedronNode* c2 = new PolyhedronNode(s4,osg::Vec3(0,0,80));
    c2->ClothActor = XCloth::Construct(pxm->gScene,pxm->gPhysicsSDK,c2,false);
    c2->ClothActor->setThickness(0.5);
    c2->WireFrame  = false;
    c2->PColor = osg::Vec3(0.85,0,0);
    pman->AddPolyhedron<ClothManager>(c2);*/

    /*PolyhedronNode* c5 = new PolyhedronNode(s5,osg::Vec3(20,20,60));
    c5->ClothActor = XCloth::Construct(pxm->gScene,pxm->gPhysicsSDK,c5,true);
    c5->ClothActor->setThickness(0.5);
    c2->ClothActor->setPressure(3.5);
    c5->WireFrame  = false;
    c5->PColor = osg::Vec3(0.85,0.85,0);
    pman->AddPolyhedron<ClothManager>(c5);*/

}