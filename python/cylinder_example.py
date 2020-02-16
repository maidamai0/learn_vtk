import vtk
from vtkFiltersSources import vtkCylinderSource
from vtkRenderingCore import vtkPolyDataMapper
from vtkRenderingCore import vtkActor
from vtkRenderingCore import vtkRenderer
from vtkRenderingCore import vtkRenderWindow
from vtkRenderingCore import vtkRenderWindowInteractor


def main():
    source = vtkCylinderSource()

    mapper = vtkPolyDataMapper()
    mapper.SetInputConnection(source.GetOutputPort())

    actor = vtkActor()
    actor.SetMapper(mapper)

    render = vtkRenderer()
    render.AddActor(actor)

    window = vtkRenderWindow()
    window.AddRenderer(render)

    interactor = vtkRenderWindowInteractor()
    interactor.SetRenderWindow(window)
    interactor.Initialize()
    window.Render()
    interactor.Start()


if __name__ == '__main__':
    main()
