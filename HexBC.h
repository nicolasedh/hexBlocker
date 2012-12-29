#ifndef HEXBC_H
#define HEXBC_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <iostream>
#include <QTextStream>


//Pre declarations
class vtkIdList;
class vtkCollection;
class hexPatch;



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

    vtkSmartPointer<vtkIdList> patchIds;
    vtkSmartPointer<vtkCollection> allPatches;
    void setPatchColors(double r,double g, double b);
    void resetPatchColors();

    // finds id of patch in global list returns true if it was found
    bool insertPatchIfIdsExists(vtkSmartPointer<vtkIdList> ids);
    std::string name;
    std::string type;
private:


};

#endif // HEXBLOCK_H
