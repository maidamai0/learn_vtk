import QtQuick 2.12
import QtQuick.Window 2.12
import VTK 9.0

Window {
    width: 640
    height: 480
    visible: true

   color: palette.window
 
    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }

    // Instantiate the vtk render window
    VTKRenderWindow {
        id: vtkwindow
        width: 400
        height: 400
    }

    // add one or more vtk render items
    VTKRenderItem {
        objectName: "ConeView"
        x: 200
        y: 200
        width: 200
        height: 200
        // Provide the handle to the render window
        renderWindow: vtkwindow
    }
}