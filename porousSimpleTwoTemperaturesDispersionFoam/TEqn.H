    
    kd=0.12*rhof*cpf*U.component(0)*dp*(1.0-exp(-mesh.C().component(1)/(1.0*dp)));
    //kd=0.025*(kfe+kse);
    kd_2=0.12*rhof*cpf*U.component(0)*dp*(1.0-exp(-(H-mesh.C().component(1))/(1.0*dp)));
        
    // set internal kd field
    
    forAll(kd, cellI)
    {  
       if( mesh.C()[cellI].component(1) > scalar(0.005) )
       {
          kd[cellI] = kd_2[cellI];
       }
    }
    
    // set boundary field
    
    forAll(kd.boundaryField(), patchI)
    {
        forAll(kd.boundaryField()[patchI], faceI)
        {
            if( mesh.C()[faceI].component(1) > scalar(0.005) )
            {
               kd.boundaryField()[patchI][faceI] = kd_2.boundaryField()[patchI][faceI];
            }    
        }
    }
    
    // update coupled boundaries
        kd.correctBoundaryConditions();

        //record updated kd
        
        if(runTime.outputTime())
        {
            kd.write();
        }
        

    // Tf treated as implicit in TfEqn, Ts treated as explicit
    fvScalarMatrix TfEqn
    (
          rhof*cpf*fvm::div(phi,Tf)
        ==
          fvm::laplacian(kfe+kd,Tf,"laplacian(DT,T)")
        - fvm::Sp(hsf*asf,Tf)
        + hsf*asf*Ts
    );

    TfEqn.relax();
    TfEqn.solve();

    // Ts treated an implicit in TsEqn, Tf treated as explicit
    fvScalarMatrix TsEqn
    (
          fvm::laplacian(kse,Ts,"laplacian(DT,T)")
        - fvm::Sp(hsf*asf,Ts)
        + hsf*asf*Tf
    );

    TsEqn.relax();
    TsEqn.solve();
