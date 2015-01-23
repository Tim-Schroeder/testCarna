 #include <QApplication>
 #include <Carna/VolumeRenderings/DRR/XRay.h>
 #include <Carna/VolumeRenderings/VolumeRenderer.h>
 #include <Carna/VolumeRenderings/DefaultVolumeVisualization.h>
 #include <Carna/dicom/DicomSceneFactory.h>
 #include <Carna/base/model/Scene.h>
 #include <Carna/base/view/SceneProvider.h>
 #include <Carna/base/qt/Display.h>

//Polyline
#include <Carna/base/view/Polyline.h>
#include <Carna/base/model/Position.h>

Carna::base::view::Polyline* doPolylineExample1( Carna::base::model::Scene& model );


 int main( int argc, char** argv )
 {
     QApplication app( argc, argv );

  // create data model

     Carna::dicom::DicomSceneFactory modelFactory;
     std::unique_ptr< Carna::base::model::Scene > model( modelFactory.createFromUserInput() );
     if( model.get() == nullptr )
     {
         return 0;
     }

  // create view
 
     Carna::VolumeRenderings::VolumeVisualization* const view
            = new Carna::VolumeRenderings::DefaultVolumeVisualization();

  // setup renderer
view->doAfterInitialization( [&]()
         {
             view->renderer().installMode( new Carna::VolumeRenderings::DRR::XRay( *model ) );

          // set initial mode

             view->renderer().setMode( Carna::VolumeRenderings::DRR::XRay::NAME );
             
             (doPolylineExample1( *model ))/*->paint( view->renderer() )*/;

         }
     );
 
  // setup view

     Carna::base::view::SceneProvider provider( *model );
     Carna::base::qt::Display* const display
         = new Carna::base::qt::Display( view, provider ); // takes ownership of 'view'

     display->setAttribute( Qt::WA_DeleteOnClose );
     display->show();


     

  // run demo

     return app.exec();
 }




 Carna::base::view::Polyline* doPolylineExample1( Carna::base::model::Scene& model )
 {
     Carna::base::view::Polyline* const line = new Carna::base::view::Polyline( model, Carna::base::view::Polyline::lineLoop );

     ( *line ) << Carna::base::model::Position::fromVolumeUnits( model, 0, 0, 0 )
               << Carna::base::model::Position::fromVolumeUnits( model, 1, 0, 0 )
               << Carna::base::model::Position::fromVolumeUnits( model, 1, 1, 0 )
               << Carna::base::model::Position::fromVolumeUnits( model, 0, 1, 0 );

     return line;
 }