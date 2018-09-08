using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
  
using OpenCvSharp;
using OpenCvSharp.Extensions;
using OpenCvSharp.Utilities;
  
namespace OpenCvTest
{
    class Program
    {
        static void Main(string[] args)
        {
            //读入源文件
            var src = IplImage.FromFile("source.jpg");
                 
            //转换到灰度图
            var gray = Cv.CreateImage(src.Size, BitDepth.U8, 1);
            Cv.CvtColor(src, gray, ColorConversion.BgrToGray);
                 
            //做一下膨胀，x与y方向都做，但系数不同
            //使用了Erode方法，腐蚀操作，针对白色区域，所以等效于对文字进行了膨胀
            var kernal = Cv.CreateStructuringElementEx(5, 2, 1, 1, ElementShape.Rect);
            Cv.Erode(gray, gray, kernal, 2);
                 
            //二值化
            Cv.Threshold(gray, gray, 0, 255, ThresholdType.BinaryInv | ThresholdType.Otsu);
                 
            //检测连通域，每一个连通域以一系列的点表示，FindContours方法只能得到第一个域
            var storage = Cv.CreateMemStorage();
            CvSeq<CvPoint> contour = null;
            Cv.FindContours(gray, storage, out contour, CvContour.SizeOf, ContourRetrieval.CComp, ContourChain.ApproxSimple);
            var color = new CvScalar(0, 0, 255);
                 
            //开始遍历
            while (contour != null)
            {
                //得到这个连通区域的外接矩形
                var rect = Cv.BoundingRect(contour);
                     
                //如果高度不足，或者长宽比太小，认为是无效数据，否则把矩形画到原图上
                if(rect.Height > 10 && (rect.Width * 1.0 / rect.Height) > 0.2)
                    Cv.DrawRect(src, rect, color);
                         
                //取下一个连通域
                contour = contour.HNext;
            }
            Cv.ReleaseMemStorage(storage);
                 
            //显示
            Cv.ShowImage("Result", src);
            Cv.WaitKey();
            Cv.DestroyAllWindows();
        }
    }
}