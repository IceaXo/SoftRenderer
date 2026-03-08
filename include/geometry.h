#pragma once

 struct Vec3f{
    float x,y,z;

    Vec3f():x(0.0f),y(0.0f),z(0.0f) {}
    Vec3f(float _x,float _y,float _z):x(_x),y(_y),z(_z){}

    Vec3f operator-(const Vec3f& other) const{
        return Vec3f(x-other.x,y-other.y,z-other.z);
    }
 };

 struct Vec4f{
   float x,y,z,w;
   Vec4f():x(0.0f),y(0.0f),z(0.0f),w(1.0f){}
   Vec4f(float _x,float _y,float _z,float _w=1.0f):x(_x),y(_y),z(_z),w(_w){}

 };

 struct Matrix4x4{
   float m[4][4];

   Matrix4x4(){
      for (int i = 0;i<4;++i){
         for (int j=0;j<4;++j){
            m[i][j] =(i==j)?1.0f:0.0f;
         }
      }
   }

   Matrix4x4 operator*(const Matrix4x4& rhs) const{
      Matrix4x4 result;
      for (int i=0;i<4;++i){
         for (int j=0;j<4;++j){
            result.m[i][j] =0.0f;
            for (int k = 0;k<4;++k){
               result.m[i][j]+=this->m[i][k]*rhs.m[k][j];
            }
         }
      }
      return result;
   }

   Vec4f operator*(const Vec4f& v) const{
      return Vec4f(
         m[0][0]*v.x+m[0][1]*v.y+m[0][2]*v.z+m[0][3]*v.w,
         m[1][0]*v.x+m[1][1]*v.y+m[1][2]*v.z+m[1][3]*v.w,
         m[2][0]*v.x+m[2][1]*v.y+m[2][2]*v.z+m[2][3]*v.w,
         m[3][0]*v.x+m[3][1]*v.y+m[3][2]*v.z+m[3][3]*v.w
      );
   }

   static Matrix4x4 MakeScale(float a,float b,float c){
      Matrix4x4 result;
      
      result.m[0][0] = a;
      result.m[1][1] = b;
      result.m[2][2] = c;
      result.m[3][3] = 1.0f;
      return result;
   }

   static Matrix4x4 MakeTranslation(float a,float b,float c){
      Matrix4x4 result;
      result.m[0][3] = a;
      result.m[1][3] = b;
      result.m[2][3] = c;
      return result;
   }

 };

// 终极顶点变换器 (等价于简化版的 Vertex Shader)
// 输入：纯净的 3D 顶点，以及要施加的矩阵
// 输出：变换完成、且执行完透视除法的纯净 3D 顶点
inline Vec3f ApplyTransform(const Vec3f& v, const Matrix4x4& mat) {
    // 1. 内部隐式升维穿马甲
    Vec4f v_4d(v.x, v.y, v.z, 1.0f);
    
    // 2. 矩阵狂暴轰炸
    Vec4f clip = mat * v_4d;
    
    // 3. 透视除法并降维，一步到位直接返回！
    return Vec3f(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
}

//直接把函数大括号里的代码，当成文本一样【粘贴】到调用它的地方
//消除了所有的函数跳跃开销
 inline float CrossProduct2D(const Vec3f& v1,const Vec3f& v2){
    return v1.x*v2.y-v1.y*v2.x;
 }