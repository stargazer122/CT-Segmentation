#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagechecking.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDICOMImageReader.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkStripper.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkMarchingCubes.h>
#include <vtkOutlineFilter.h>
#include <vtkProperty.h>

#include <vtkImageViewer2.h>

vtkRenderWindow* MainWindow::renWin;
QString MainWindow::fileName="";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::lily(const QString &name,const int &level) //当显示为空白时，调节value，并且要控制读入图像的数量
{
    QByteArray array=name.toLatin1();
    char *pathName=array.data();
    int value=level;
     vtkRenderer* ren=vtkRenderer::New();             //设置绘制者(绘制对象指针)

     renWin=vtkRenderWindow::New();       //设置绘制窗口
     renWin->AddRenderer(ren);                                 //将绘制者加入绘制窗口
     //renWin->SetFullScreen(1);

     vtkRenderWindowInteractor* iren=vtkRenderWindowInteractor::New();//设置绘制交互操作窗口的
     iren->SetRenderWindow(renWin);

    vtkDICOMImageReader* Reader=vtkDICOMImageReader::New();

        Reader->SetDirectoryName(pathName);
        Reader->SetDataByteOrderToLittleEndian();
        //Reader->SetDataSpacing(1,1,1);
        Reader->Update();
   vtkMarchingCubes* marchingcube=vtkMarchingCubes::New();       //建立一个Marching Cubes 算法的对象
        //marchingcube->SetInput((vtkDataSet *)Reader->GetOutput());      //获得所读取的数据
        marchingcube->SetInputConnection(Reader->GetOutputPort());      //第二种读取数据的方法
        marchingcube->SetValue(0,value);                                  //140提取出灰度值为45的东西Lindsay！！！
        //marchingcube->Update();

     vtkStripper* Stripper=vtkStripper::New();                   //建立三角带对象
       Stripper->SetInputConnection(marchingcube->GetOutputPort() );              //将生成的三角片连接成三角带
       //Stripper->Update();

     vtkPolyDataMapper* Mapper=vtkPolyDataMapper::New();   //建立一个数据映射对象
          Mapper->SetInputConnection(Stripper->GetOutputPort() );            //将三角带映射为几何数据
          Mapper->ScalarVisibilityOff();
          //Mapper->Update();

      vtkActor* actor=vtkActor::New();                            //建立一个代表皮肤的演员
            actor->SetMapper(Mapper);                                  //获得皮肤几何数据的属性
            actor->GetProperty()->SetDiffuseColor(1, .49, .25);            //设置皮肤颜色的属性//(1, .49, .25)
            //actor->GetProperty()->SetDiffuseColor(1, 1, .25);
            actor->GetProperty()->SetSpecular(0.3);                         //设置反射率
            actor->GetProperty()->SetSpecularPower(20);                    //设置反射光强度
            actor->GetProperty()->SetOpacity(1.0);
            actor->GetProperty()->SetColor(1,0,0);                 //设置角色的颜色
            actor->GetProperty()->SetRepresentationToWireframe();


      vtkCamera* aCamera=vtkCamera::New();     // 创建摄像机
            aCamera->SetViewUp ( 0, 0, -1 );        //设置相机的“上”方向
            aCamera->SetPosition ( 0, 1, 0 );       //位置：世界坐标系，设置相机位置
            aCamera->SetFocalPoint( 0, 0, 0 );     //焦点：世界坐标系，控制相机方向
            aCamera->ComputeViewPlaneNormal();     //重置视平面法向，基于当前的位置和焦点。否则会出现斜推剪切效果
            ///////////////////////////////////////
     vtkOutlineFilter* outlinefilter=vtkOutlineFilter::New();
            outlinefilter->SetInputConnection(Reader->GetOutputPort());
            //outlinefilter->Update();//

      vtkPolyDataMapper* outlineMapper=vtkPolyDataMapper::New();
            outlineMapper->SetInputConnection(outlinefilter->GetOutputPort());
            //outlineMapper->Update();//

       vtkActor* OutlineActor=vtkActor::New();
            OutlineActor->SetMapper(outlineMapper);
            OutlineActor->GetProperty()->SetColor(0,0,0);
            // 告诉绘制者将要在绘制窗口中进行显示的演员
            ren->AddActor(actor);
            ren->AddActor(OutlineActor);

            ren->SetActiveCamera(aCamera);               ////设置渲染器的相机
            ren->ResetCamera();
            aCamera->Dolly(1.5);                //使用Dolly()方法沿着视平面法向移动相机，实现放大或缩小可见角色物体
            ren->SetBackground(1,1,1);               //设置背景颜色
            //ren->ResetCameraClippingRange();

            renWin->SetSize(1000, 600);
            renWin->Render();
            iren->Initialize();
            iren->Start();
            std::cout<<"show!"<<endl;

/*----------程序运行到这里会等待关闭键，然后继续执行---------------------------------*/
/*            ren->Delete();
            renWin->Delete();
            iren->Delete();
            Reader->Delete();
            marchingcube->Delete();
            Stripper->Delete();
            Mapper->Delete();
            actor->Delete();
            aCamera->Delete();
            outlinefilter->Delete();
            outlineMapper->Delete();
            OutlineActor->Delete();*/
            std::cout<<"end!"<<endl;
      /*vtkSmartPointer<vtkOBJExporter> porter=vtkSmartPointer<vtkOBJExporter>::New();
            porter->SetFilePrefix("E:\\PolyDataWriter.obj");
            porter->SetInput(renWin);
            porter->Write();  */

}

void MainWindow::on_open_clicked()
{

    QFileDialog *dialog=new QFileDialog;
    fileName=dialog->getExistingDirectory(this,tr("请选择文件"),"");
    dialog->activateWindow();
    if(!fileName.isEmpty()&&(dialog->Accepted))
    {
       ImageChecking check(fileName);
       int t1=check.Checking_empty_type();
       int t2=check.Checking_size();
       int value=ui->spinBox->value();
       if(t1&&t2) lily(fileName,value);
    }
}


void MainWindow::on_refresh_clicked()
{
    vtkSmartPointer<vtkDICOMImageReader> reader =vtkSmartPointer<vtkDICOMImageReader>::New();
       reader->SetFileName("D:/TTest/01.dcm");
       reader->Update();

       // Visualize
       vtkSmartPointer<vtkImageViewer2> imageViewer=vtkSmartPointer<vtkImageViewer2>::New();
       imageViewer->SetInputConnection(reader->GetOutputPort());
       vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor=vtkSmartPointer<vtkRenderWindowInteractor>::New();
       imageViewer->SetupInteractor(renderWindowInteractor);
       imageViewer->Render();
       imageViewer->GetRenderer()->ResetCamera();
       imageViewer->Render();

       renderWindowInteractor->Start();


}
