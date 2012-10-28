#ifndef HEXBLOCK_H
#define HEXBLOCK_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <iostream>

//Pre declarations
class vtkIdList;
class vtkCollection;
class QString;


class HexBC : public vtkObject
{
vtkTypeMacro(HexBC,vtkObject)

protected:
    HexBC();
    HexBC(QString* nam,QString* typ);
    ~HexBC();
    HexBC(const HexBC&);  // Not implemented in order to comply with vtkObject.
    void operator=(const HexBC&);  // Not implemented in order to comply with vtkObject.

public:
    static HexBC *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    vtkSmartPointer<vtkIdList> patchIds;
    vtkSmartPointer<vtkCollection> allPatches;


    QString *name;
    QString *type;
private:


};

#endif // HEXBLOCK_H
