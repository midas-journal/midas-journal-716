#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAxesActor.h>
#include <vtkObjectFactory.h>

#include <vtksys/ios/sstream>

#include "vtkVoodooReader.h"
#include "vtkCalibratedImageCollection.h"
#include "vtkImageCamera.h"

class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeRevisionMacro(KeyPressInteractorStyle,vtkInteractorStyleTrackballCamera);
    
    KeyPressInteractorStyle()
    {
      this->CurrentCameraId = 0;
    }
    
    virtual void OnKeyPress() 
    {
      //get the keypress
      vtkRenderWindowInteractor *rwi = this->Interactor;
      std::string key = rwi->GetKeySym();
             
      if(key.compare("Left") == 0 && this->CurrentCameraId > 0)
        {
        cout << "The left arrow was pressed." << endl;
        this->ImageCollection->GetCamera(CurrentCameraId)->GetAxesActor()->VisibilityOff();
        this->ImageCollection->GetCamera(CurrentCameraId)->GetImageActor()->VisibilityOff();
        CurrentCameraId--;
        this->ImageCollection->GetCamera(CurrentCameraId)->GetAxesActor()->VisibilityOn();
        this->ImageCollection->GetCamera(CurrentCameraId)->GetImageActor()->VisibilityOn();
        }
        
      if(key.compare("Right") == 0 && this->CurrentCameraId < this->ImageCollection->GetNumberOfCameras() - 1)
        {
        cout << "The right arrow was pressed." << endl;
        this->ImageCollection->GetCamera(CurrentCameraId)->GetAxesActor()->VisibilityOff();
        this->ImageCollection->GetCamera(CurrentCameraId)->GetImageActor()->VisibilityOff();
        CurrentCameraId++;
        this->ImageCollection->GetCamera(CurrentCameraId)->GetAxesActor()->VisibilityOn();
        this->ImageCollection->GetCamera(CurrentCameraId)->GetImageActor()->VisibilityOn();
        }
      
      if(key.compare("Up") == 0)
        {
        //this->ImageCollection->GetCamera(CurrentCameraId)->SetDistanceFromCamera();
        }
      cout << "Displaying camera: " << this->CurrentCameraId << endl;
      this->RenderWindow->Render();
      
      // forward events
      vtkInteractorStyleTrackballCamera::OnKeyPress();
    }
 
    void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renderWindow) {this->RenderWindow = renderWindow;}
    void SetImageCollection(vtkSmartPointer<vtkCalibratedImageCollection> imageCollection) {this->ImageCollection = imageCollection;}
  private:
    vtkSmartPointer<vtkRenderWindow> RenderWindow;
    unsigned int CurrentCameraId;
    vtkSmartPointer<vtkCalibratedImageCollection> ImageCollection;
};

vtkStandardNewMacro(KeyPressInteractorStyle);
vtkCxxRevisionMacro(KeyPressInteractorStyle, "$Revision: 1.70 $");
 
int main (int argc, char *argv[])
{
  //verify input arguments
  if(argc != 3)
    {
    cout << "Required arguments: InputFilename.voodoo ImageDirectory" << endl;
    return EXIT_FAILURE;
    }
  
  //parse commandline arguments
  vtkstd::string inputFilename = argv[1];
  vtkstd::string inputImageDirectory = argv[2];
  
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
  
  vtkSmartPointer<KeyPressInteractorStyle> style = 
      vtkSmartPointer<KeyPressInteractorStyle>::New();
  renderWindowInteractor->SetInteractorStyle( style );
  style->SetRenderWindow(renderWindow);
  renderWindow->SetSize(800,800);
  //renderer->SetBackground(1,1,1); // Background color white
   
  //create a voodoo reader
  vtkSmartPointer<vtkVoodooReader> reader = 
      vtkSmartPointer<vtkVoodooReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->SetImageDirectory(inputImageDirectory.c_str());
  reader->Update();
  
  //add the cameras/images to the scene
  //vtkCalibratedImageCollection* imageCollection = reader->GetOutputImageCollection();
  vtkCalibratedImageCollection* imageCollection = reader->GetCameras();
  cout << "There are " << imageCollection->GetNumberOfCameras() << " cameras." << endl;
  style->SetImageCollection(imageCollection);
  
  for(unsigned int i = 0; i < imageCollection->GetNumberOfCameras(); i++)
    {
    vtkImageCamera* camera = imageCollection->GetCamera(i);
    //cout << "Camera " << i << endl << *camera << endl;
    camera->SetRenderer(renderer);
    camera->SetDistanceFromCamera(6);
    camera->UpdateCameraActor();
    camera->UpdateImageActor();
    camera->GetAxesActor()->VisibilityOff();
    camera->GetImageActor()->VisibilityOff();
    std::stringstream ssX;
    ssX << "X" << i;
    std::stringstream ssY;
    ssY << "Y" << i;
    std::stringstream ssZ;
    ssZ << "Z" << i;
    camera->GetAxesActor()->SetXAxisLabelText(ssX.str().c_str());
    camera->GetAxesActor()->SetYAxisLabelText(ssY.str().c_str());
    camera->GetAxesActor()->SetZAxisLabelText(ssZ.str().c_str());
    
    }
  
    imageCollection->GetCamera(0)->GetAxesActor()->VisibilityOn();
    imageCollection->GetCamera(0)->GetImageActor()->VisibilityOn();
  
  //add the points to the scene
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
      vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetPoints()->GetProducerPort());
  vtkSmartPointer<vtkActor> actor = 
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  renderer->AddActor(actor);
 
  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
  
  return EXIT_SUCCESS;
}
