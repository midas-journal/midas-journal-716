// .NAME vtkCalibratedImageCollection - Store a set of images and calibration parameters.
// .SECTION Description
// Store extrinsic and intrinsic parameters and associated images.

#ifndef __vtkCalibratedImageCollection_h
#define __vtkCalibratedImageCollection_h

#include "vtkDataObject.h"
#include "vtkSmartPointer.h"

#include <vtkstd/vector>
#include <vtkstd/string>

class vtkInformation;
class vtkInformationVector;
class vtkImageCamera;

class vtkCalibratedImageCollection : public vtkDataObject
{
  public:
    vtkTypeRevisionMacro(vtkCalibratedImageCollection,vtkObject);
    void PrintSelf(ostream& os, vtkIndent indent);
    static vtkCalibratedImageCollection *New();
    void DeepCopy(vtkCalibratedImageCollection*);
    
    vtkImageCamera* GetCamera(const unsigned int i);
    vtkstd::vector<vtkSmartPointer<vtkImageCamera> > GetCameras();
        
    void AddCamera(vtkImageCamera*);
    
    unsigned int GetNumberOfCameras();
    
  protected:
    vtkCalibratedImageCollection();
    ~vtkCalibratedImageCollection();
  
  private:
    vtkCalibratedImageCollection(const vtkCalibratedImageCollection&);  // Not implemented.
    void operator=(const vtkCalibratedImageCollection&);  // Not implemented.
    
    vtkstd::vector<vtkSmartPointer<vtkImageCamera> > Cameras;

};

#endif