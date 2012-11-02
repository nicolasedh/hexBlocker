#ifndef HEXBC_H
#define HEXBC_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <iostream>

//Pre declarations
class vtkIdList;
class vtkCollection;



class HexBC : public vtkObject
{
vtkTypeMacro(HexBC,vtkObject)

protected:
    HexBC();
    ~HexBC();
    HexBC(const HexBC&);  // Not implemented in order to comply with vtkObject.
    void operator=(const HexBC&);  // Not implemented in order to comply with vtkObject.

public:
    static HexBC *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    vtkIdList *patchIds;
    vtkSmartPointer<vtkCollection> allPatches;


    std::string name;
    std::string type;
private:


};

#endif // HEXBLOCK_H
