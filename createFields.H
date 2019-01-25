    Info<< "Reading field p\n" << endl;
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );
    
    Info<< "Reading field Ts\n" << endl;
    volScalarField Ts
    (
        IOobject
        (
            "Ts",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );


    Info<< "Reading field Tf\n" << endl;
    volScalarField Tf
    (
        IOobject
        (
            "Tf",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field kd\n" << endl;
    volScalarField kd
    (
        IOobject
        (
        "kd",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("kd",dimensionSet(1,1,-3,-1,0),0.0)
    );

    Info<< "Reading field kd_2\n" << endl;
    volScalarField kd_2
    (
        IOobject
        (
        "kd_2",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("kd_2",dimensionSet(1,1,-3,-1,0),0.0)
    );

    #include "createPhi.H"


    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell(p, mesh.solutionDict().subDict("SIMPLE"), pRefCell, pRefValue);

    singlePhaseTransportModel laminarTransport(U, phi);

    Info<< "Reading transportProperties\n" << endl;

    IOdictionary transportProperties
    (
        IOobject
	(
	    "transportProperties",
	    runTime.constant(),
	    mesh,
	    IOobject::MUST_READ,
	    IOobject::NO_WRITE
	)
    );

    Info<< "Reading fluid kinematic viscosity nulaminar\n" << endl;

    dimensionedScalar nu
    (
        transportProperties.lookup("nu")
    );

    Info<< "Reading porosity eps\n" << endl;

    dimensionedScalar eps
    (
        transportProperties.lookup("eps")
    );
    
    Info<< "Reading effective fluid conductivity kfe\n" << endl;

    dimensionedScalar kfe
    (
        transportProperties.lookup("kfe")
    );
    
    Info<< "Reading effective solid conductivity kse\n" << endl;

    dimensionedScalar kse
    (
        transportProperties.lookup("kse")
    );    
    
    Info<< "Reading fluid density rhof\n" << endl;

    dimensionedScalar rhof
    (
        transportProperties.lookup("rhof")
    );
    
    Info<< "Reading fluid specific heat capacity cpf\n" << endl;

    dimensionedScalar cpf
    (
        transportProperties.lookup("cpf")
    );
    
    Info<< "Reading u_mean\n" << endl;

    dimensionedScalar u_mean
    (
        transportProperties.lookup("u_mean")
    );

    Info<< "Reading surface area density asf\n" << endl;

    dimensionedScalar asf
    (
        transportProperties.lookup("asf")
    );

    Info<< "Reading interfacial heat transfer coefficient hsf\n" << endl;

    dimensionedScalar hsf
    (
        transportProperties.lookup("hsf")
    );

    Info<< "Reading particle diameter dp\n" << endl;

    dimensionedScalar dp
    (
        transportProperties.lookup("dp")
    );

    dimensionedScalar H
    (
        "H",
        dimensionSet(0,1,0,0,0,0,0),
        scalar(0.01)
    );


    autoPtr<incompressible::RASModel> turbulence
    (
        incompressible::RASModel::New(U, phi, laminarTransport)
    );
