
#include "qapplication.h"
#include "qcolor.h"
#include "qdesktopservices.h"
#include "qimage.h"
#include "qrgb.h"
#include "qurl.h"

#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "vtkType.h"

#include "../../platform.h"
#include <filesystem>
#include <iostream>

auto create_demo_data() {
  const auto image = vtkSmartPointer<vtkImageData>::New();
  image->SetDimensions(1000, 1000, 1);
  image->AllocateScalars(VTK_UNSIGNED_CHAR, 3);

  const auto width = image->GetDimensions()[0];
  const auto height = image->GetDimensions()[1];

  for (int x = 0; x < height; ++x) {
    for (int y = 0; y < width; ++y) {
      const auto pixel
        = static_cast<unsigned char*>(image->GetScalarPointer(x, y, 0));
      pixel[0] = 0;
      pixel[1] = 255;
      pixel[2] = 0;
    }
  }

  return image;
}

auto image_data_to_qimage(const vtkSmartPointer<vtkImageData>& image_data) {
  if (image_data == nullptr) {
    return QImage{};
  }

  const auto width = image_data->GetDimensions()[0];
  const auto height = image_data->GetDimensions()[1];

  QImage qimage(width, height, QImage::Format_RGB32);

  auto rgb_ptr = reinterpret_cast<QRgb*>(qimage.bits()) + width * (height - 1);
  auto color_ptr
    = reinterpret_cast<unsigned char*>(image_data->GetScalarPointer());

  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      *(rgb_ptr++) = QColor(color_ptr[0], color_ptr[1], color_ptr[2]).rgb();
      color_ptr += image_data->GetNumberOfScalarComponents();
    }

    rgb_ptr -= width * 2;
  }

  return qimage;
}

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  auto qimage = image_data_to_qimage(create_demo_data());

  auto path = QString::fromStdString(
    fs::current_path().append("build\\qimage.png").string());
  std::cout << "image path is " << path.toStdString() << std::endl;
  qimage.save(path);
  return 0;
}