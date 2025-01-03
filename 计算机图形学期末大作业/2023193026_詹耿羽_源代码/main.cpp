#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"

#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>


int WIDTH = 600;
int HEIGHT = 600;
int mainWindow;
const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;
int Axis = X_AXIS;



class MatrixStack {
	int		_index;
    int		_size;
    glm::mat4*	_matrices;

public:
	MatrixStack(int numMatrices = 100):_index(0), _size(numMatrices)
        { _matrices = new glm::mat4[numMatrices]; }

    ~MatrixStack()
		{ delete[]_matrices; }

    void push(const glm::mat4& m){
		assert( _index + 1 < _size );
		_matrices[_index++] = m;	 
    }

	glm::mat4& pop(){
        assert(_index - 1 >= 0);
        _index--;
        return _matrices[_index];
    }
};

#define White	glm::vec3(1.0, 1.0, 1.0)
#define Yellow	glm::vec3(1.0, 1.0, 0.0)
#define Green	glm::vec3(0.0, 1.0, 0.0)
#define Cyan	glm::vec3(0.0, 1.0, 1.0)
#define Magenta	glm::vec3(1.0, 0.0, 1.0)
#define Red		glm::vec3(1.0, 0.0, 0.0)
#define Black	glm::vec3(0.0, 0.0, 0.0)
#define Blue	glm::vec3(0.0, 0.0, 1.0)
#define Brown	glm::vec3(0.5, 0.5, 0.5)

struct Robot
{
	// 关节大小
	float TORSO_HEIGHT = 4.0;
	float TORSO_WIDTH = 2.5;
	float UPPER_ARM_HEIGHT = 1.7;
	float LOWER_ARM_HEIGHT = 3.2;
	float UPPER_ARM_WIDTH =  0.8;
	float LOWER_ARM_WIDTH =  0.5;
	float UPPER_LEG_HEIGHT = 1.8;
	float LOWER_LEG_HEIGHT = 4.0;
	float UPPER_LEG_WIDTH =  1.0;
	float LOWER_LEG_WIDTH =  0.5;
	float HEAD_HEIGHT = 1.8;
	float HEAD_WIDTH = 1.5;
	float DEEPTH = 0.2;	//剑宽
	float JIANBING_HIGHT = 1.5; //剑柄
	float JIANBING_WIDTH = 0.5;
	float JIANGE_HIGHT = 0.3; //剑格
	float JIANGE_WIDTH = 1.2;
	float JIANSHEN_HIGHT = 2.4;	//剑身
	float JIANSHEN_WIDTH = 0.6;
	float JIANJIAN_HIGHT = 0.7;	//剑尖
	float JIANJIAN_WIDTH = 0.6;

	// 关节角和菜单选项值
	enum {
		Torso,			// 躯干
		Head,			// 头部
		RightUpperArm,	// 右大臂
		RightLowerArm,	// 右小臂
		LeftUpperArm,	// 左大臂
		LeftLowerArm,	// 左小臂
		RightUpperLeg,	// 右大腿
		RightLowerLeg,	// 右小腿
		LeftUpperLeg,	// 左大腿
		LeftLowerLeg,	// 左小腿
		JIANBING,		//剑柄
		JIANGE,			//剑格
		JIANSHEN,		//剑身
		JIANJIAN,		//剑尖
		Ground			//地面
	};
	glm::vec3 move_offect = glm::vec3(0.0,0.0,0.0);

	// 关节角大小
	glm::vec3 theta[14] = {
		glm::vec3(0.0),    	// Torso
		glm::vec3(0.0),    	// Head
		glm::vec3(0.0),   	 // RightUpperArm
		glm::vec3(0.0),   	 // RightLowerArm
		glm::vec3(0.0),   	 // LeftUpperArm
		glm::vec3(0.0),    	// LeftLowerArm
		glm::vec3(0.0),   	 // RightUpperLeg
		glm::vec3(0.0),   	 // RightLowerLeg
		glm::vec3(0.0),    	// LeftUpperLeg
		glm::vec3(0.0),     // LeftLowerLeg
		glm::vec3(0.0),		//JIANBING
		glm::vec3(0.0),		//JIANGE
		glm::vec3(0.0),		//JIANSHEN
		glm::vec3(0.0)		//JIANJIAN
	};
};
Robot robot;
// 被选中的物体
int Selected_mesh = robot.Torso;


TriMesh* Torso = new TriMesh();
TriMesh* Head = new TriMesh();
TriMesh* RightUpperArm = new TriMesh();
TriMesh* RightLowerArm = new TriMesh();
TriMesh* LeftUpperArm = new TriMesh();
TriMesh* LeftLowerArm = new TriMesh();
TriMesh* RightUpperLeg = new TriMesh();
TriMesh* RightLowerLeg = new TriMesh();
TriMesh* LeftUpperLeg = new TriMesh();
TriMesh* LeftLowerLeg = new TriMesh();
TriMesh* JIANBING = new TriMesh();
TriMesh* JIANGE = new TriMesh();
TriMesh* JIANSHEN = new TriMesh();
TriMesh* JIANJIAN = new TriMesh();
TriMesh* Ground = new TriMesh();



Camera* camera = new Camera();
Light* light = new Light();
MeshPainter* painter = new MeshPainter();

std::vector<openGLObject> objects; 

// 获取生成的所有模型，用于结束程序时释放内存
std::vector<TriMesh*> meshList;
float ProjectionPos = -robot.UPPER_LEG_HEIGHT-robot.LOWER_LEG_HEIGHT; //设置投影平面高度

//bool isWaveActive = false;
//float waveRadius = 0.0f;
//glm::vec3 skyColor(0.5, 0.7, 1.0); // 初始颜色（天蓝）
bool isClapping = false;
float clapProgress = 0.0f;
//
void teleport(float distance, int axis) {
	if (axis == X_AXIS) {
		robot.move_offect.x += distance; // 瞬移在 X 轴
	}
	else if (axis == Z_AXIS) {
		robot.move_offect.z += distance; // 瞬移在 Z 轴
	}
}


//


bool isJumping = false; // 跳跃状态
float jumpHeight = 3.0f; // 跳跃高度
float jumpSpeed = 5.0f;  // 跳跃速度
float jumpProgress = 0.0f; // 跳跃进度

void jump(float deltaTime) {
	if (isJumping) {
		// 跳跃过程：正弦曲线模拟上升和下降
		jumpProgress += deltaTime * jumpSpeed;
		robot.move_offect.y = jumpHeight * sin(jumpProgress);

		// 跳跃完成，恢复初始状态
		if (jumpProgress >= M_PI) { // sin(M_PI) = 0，意味着回到地面
			robot.move_offect.y = 0.0f;
			isJumping = false;
			jumpProgress = 0.0f;
		}
	}
}
void clap() {
	if (!isClapping) {
		isClapping = true;
		clapProgress = 0.0f;
	}
}

// 风车模型
TriMesh* WindmillPole = new TriMesh();  // 风车杆

TriMesh* WindmillBlade = new TriMesh(); // 风车扇叶

// 风车的绘制函数
void drawWindmill(glm::mat4 modelMatrix, float bladeAngle) {
	// 绘制杆子
	//glm::mat4 poleMatrix = glm::mat4(1.0);
	//poleMatrix = glm::scale(poleMatrix, glm::vec3(0.2, 5.0, 0.2));
	//painter->drawMesh(modelMatrix * poleMatrix, WindmillPole, objects[robot.Ground], light, camera, 0);

	// 绘制四片扇叶
	for (int i = 0; i < 4; ++i) {
		glm::mat4 bladeMatrix = glm::mat4(1.0);
		bladeMatrix = glm::translate(bladeMatrix, glm::vec3(0.0, 5.0, 0.0)); // 放置在杆子顶部
		bladeMatrix = glm::rotate(bladeMatrix, glm::radians(bladeAngle + i * 90.0f), glm::vec3(0.0, 0.0, 1.0)); // 旋转
		bladeMatrix = glm::translate(bladeMatrix, glm::vec3(1.0, 0.0, 0.0)); // 扇叶延伸位置
		bladeMatrix = glm::scale(bladeMatrix, glm::vec3(2.5, 0.42, 0.2));    // 扇叶大小
		painter->drawMesh(modelMatrix * bladeMatrix, WindmillBlade, objects[robot.Ground], light, camera, 0);
	}
}

void turn(float angle) {
	robot.theta[robot.Torso].y += angle;
	if (robot.theta[robot.Torso].y > 360.0)
		robot.theta[robot.Torso].y -= 360.0;
}
// 躯体
void torso(glm::mat4 modelMatrix)
{
	// 本节点局部变换矩阵
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, 0.5 * robot.TORSO_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.TORSO_WIDTH, robot.TORSO_HEIGHT, robot.TORSO_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	painter->drawMesh(modelMatrix * instance, Torso, objects[robot.Torso] , light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, Torso, objects[robot.Torso], light, camera, ProjectionPos);

}

// 头部
void head(glm::mat4 modelMatrix)
{	
	// 本节点局部变换矩阵
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, 0.5 * robot.HEAD_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.HEAD_WIDTH, robot.HEAD_HEIGHT, robot.HEAD_WIDTH));

	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh(modelMatrix * instance, Head, objects[robot.Head], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, Head, objects[robot.Head], light, camera, ProjectionPos);
	// 本节点局部变换矩阵
	//glm::mat4 instance = glm::mat4(1.0);
	//instance = glm::translate(instance, glm::vec3(0.0, 0.5 * robot.HEAD_HEIGHT, 0.0));
	//instance = glm::scale(instance, glm::vec3(robot.HEAD_WIDTH, robot.HEAD_HEIGHT, robot.HEAD_WIDTH));

	// 启用贴图绘制头部
	//painter->drawMesh(modelMatrix * instance, Head, objects[robot.Head], light, camera, 1); // 1 表示启用贴图
	//painter->drawShadow(modelMatrix * instance, Head, objects[robot.Head], light, camera, ProjectionPos);

}


// 左大臂
void left_upper_arm(glm::mat4 modelMatrix)
{
    // 本节点局部变换矩阵
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, -0.5 * robot.UPPER_ARM_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.UPPER_ARM_WIDTH, robot.UPPER_ARM_HEIGHT, robot.UPPER_ARM_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	painter->drawMesh( modelMatrix * instance, LeftUpperArm, objects[robot.LeftUpperArm], light, camera,1);	
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, LeftUpperArm, objects[robot.LeftUpperArm], light, camera, ProjectionPos);
}


//  左小臂
void left_lower_arm(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, -0.5 * robot.LOWER_ARM_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.LOWER_ARM_WIDTH, robot.LOWER_ARM_HEIGHT, robot.LOWER_ARM_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh( modelMatrix * instance, LeftLowerArm,objects[robot.LeftLowerArm], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, LeftLowerArm, objects[robot.LeftLowerArm], light, camera, ProjectionPos);
}

//  右大臂
void right_upper_arm(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, -0.5 * robot.UPPER_ARM_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.UPPER_ARM_WIDTH, robot.UPPER_ARM_HEIGHT, robot.UPPER_ARM_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh( modelMatrix * instance, RightUpperArm, objects[robot.RightUpperArm], light, camera,1);	
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, RightUpperArm, objects[robot.RightUpperArm], light, camera, ProjectionPos);
}

//  右小臂
void right_lower_arm(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, -0.5 * robot.LOWER_ARM_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.LOWER_ARM_WIDTH, robot.LOWER_ARM_HEIGHT, robot.LOWER_ARM_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh( modelMatrix * instance, RightLowerArm,objects[robot.RightLowerArm], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, RightLowerArm, objects[robot.RightLowerArm], light, camera, ProjectionPos);
}

//  左大腿
void left_upper_leg(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, -0.5 * robot.UPPER_LEG_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.UPPER_LEG_WIDTH, robot.UPPER_LEG_HEIGHT, robot.UPPER_LEG_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh( modelMatrix * instance, LeftUpperLeg, objects[robot.LeftUpperLeg], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, LeftUpperLeg, objects[robot.LeftUpperLeg], light, camera, ProjectionPos);
}

// 左小腿
void left_lower_leg(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, -0.5 * robot.LOWER_LEG_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.LOWER_LEG_WIDTH, robot.LOWER_LEG_HEIGHT, robot.LOWER_LEG_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh( modelMatrix * instance, LeftLowerLeg, objects[robot.LeftLowerLeg], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, LeftLowerLeg, objects[robot.LeftLowerLeg], light, camera, ProjectionPos);

}

//  右大腿
void right_upper_leg(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, -0.5 * robot.UPPER_LEG_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.UPPER_LEG_WIDTH, robot.UPPER_LEG_HEIGHT, robot.UPPER_LEG_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh( modelMatrix * instance, RightUpperLeg, objects[robot.RightUpperLeg], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, RightUpperLeg, objects[robot.RightUpperLeg], light, camera, ProjectionPos);
}

//  右小腿
void right_lower_leg(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, -0.5 * robot.LOWER_LEG_HEIGHT, 0.0));
	instance = glm::scale(instance, glm::vec3(robot.LOWER_LEG_WIDTH, robot.LOWER_LEG_HEIGHT, robot.LOWER_LEG_WIDTH));

	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh( modelMatrix * instance, RightLowerLeg, objects[robot.RightLowerLeg], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, RightLowerLeg, objects[robot.RightLowerLeg], light, camera, ProjectionPos);
}

//剑柄
void jianbing(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0,0.5*robot.JIANBING_HIGHT,0.0));
	instance = glm::scale(instance, glm::vec3(robot.DEEPTH,robot.JIANBING_HIGHT,robot.JIANBING_WIDTH));
	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh(modelMatrix * instance, JIANBING, objects[robot.JIANBING], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix *instance, JIANBING, objects[robot.JIANBING], light, camera, ProjectionPos);
}

//剑格
void jiange(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::scale(instance, glm::vec3(robot.DEEPTH, robot.JIANGE_HIGHT, robot.JIANGE_WIDTH));
	//将texopen设为0， 表示不绘制纹理
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	painter->drawMesh(modelMatrix * instance, JIANGE, objects[robot.JIANGE], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, JIANGE, objects[robot.JIANGE], light, camera, ProjectionPos);
}

//剑身
void jianshen(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::scale(instance, glm::vec3(robot.DEEPTH, robot.JIANSHEN_HIGHT, robot.JIANSHEN_WIDTH));
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	//将texopen设为0， 表示不绘制纹理
	painter->drawMesh(modelMatrix*instance, JIANSHEN, objects[robot.JIANSHEN], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, JIANSHEN, objects[robot.JIANSHEN], light, camera, ProjectionPos);
}

//剑尖
void jianjian(glm::mat4 modelMatrix)
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::scale(instance, glm::vec3(robot.DEEPTH, robot.JIANJIAN_HIGHT, robot.JIANJIAN_WIDTH));
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	//将texopen设为0， 表示不绘制纹理
	painter->drawMesh(modelMatrix*instance, JIANJIAN, objects[robot.JIANJIAN], light, camera,1);
	//绘制阴影
	painter->drawShadow(modelMatrix*instance, JIANJIAN, objects[robot.JIANJIAN], light, camera, ProjectionPos);

}
void ground()
{
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, ProjectionPos, 0.0));
	instance = glm::scale(instance, glm::vec3(30,0.0,30));
	//drawmesh()最后一位数字表示是否绘制纹理，0不绘制，1绘制
	//将texopen设为1， 表示绘制纹理
	painter->drawMesh(instance, Ground, objects[robot.Ground], light, camera,1);
}


void setmatrial_Bronze(TriMesh* mesh)
{
	mesh->setAmbient(glm::vec4(0.2125f, 0.1275f, 0.054f, 1.0));  // 环境光
	mesh->setDiffuse(glm::vec4(0.714f, 0.4824f, 0.18144f, 1.0));  // 漫反射
	mesh->setSpecular(glm::vec4(0.393548f, 0.271906f, 0.166721f, 1.0)); // 镜面反射
	mesh->setShininess(25.6f);
}
void setmatrial_Brass(TriMesh* mesh)
{

	mesh->setAmbient(glm::vec4( 0.329412f, 0.223529f, 0.027451f,1.0f));  // 环境光
	mesh->setDiffuse(glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f ));  // 漫反射
	mesh->setSpecular(glm::vec4(0.992157f, 0.941176f, 0.807843f, 1.0f)); // 镜面反射
	mesh->setShininess(27.8974f);
}
void setmatrial_Copper(TriMesh* mesh)
{
	mesh->setAmbient(glm::vec4(  0.19125f, 0.0735f, 0.0225f, 1.0f));  // 环境光
	mesh->setDiffuse(glm::vec4(0.7038f, 0.27048f, 0.0828f, 1.0f ));  // 漫反射
	mesh->setSpecular(glm::vec4(0.256777f, 0.137622f, 0.086014f, 1.0f)); // 镜面反射
	mesh->setShininess(12.8f);
}

void setmatrial_Perl(TriMesh* mesh)
{
	mesh->setAmbient(glm::vec4(  0.25f, 0.20725f, 0.20725f, 0.922f));  // 环境光
	mesh->setDiffuse(glm::vec4(1.0f, 0.829f, 0.829f, 0.922f ));  // 漫反射
	mesh->setSpecular(glm::vec4(0.296648f, 0.296648f, 0.296648f, 0.922f)); // 镜面反射
	mesh->setShininess(11.264f);
}


void init()
{
	std::string vshader, fshader;
	// 读取着色器并使用
	vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";

	//
	// 
	// 设置纹理路径
	std::string endermanTexture = "./assets/enderman_texture.jpg";

	// 设置光源位置
	light->setTranslation(glm::vec3(0.0, 5.0, 3.0));
	light->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	light->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	light->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	light->setAttenuation(1.0, 0.014, 0.0007); // 衰减系数


	// 设置物体的大小（初始的旋转和位移都为0）
	Torso->generateCube(White);


	Head->generateCube(White);
	RightUpperArm->generateCube(White);
	LeftUpperArm->generateCube(White);
	RightUpperLeg->generateCube(White);
	LeftUpperLeg->generateCube(White);
	RightLowerArm->generateCube(Black);
	LeftLowerArm->generateCube(Black);
	RightLowerLeg->generateCube(Black);
	LeftLowerLeg->generateCube(Black);
	JIANBING->generateCube(Green);
	JIANGE->generateCube(Green);
	JIANSHEN->generateCube(Green);
	JIANJIAN->generatePyramid(Green);
	Ground->generateGround(Yellow);



	//设置物体材质

	setmatrial_Copper(Torso);
	setmatrial_Copper(Head);
	setmatrial_Copper(RightUpperArm);
	setmatrial_Copper(LeftUpperArm);
	setmatrial_Copper(RightLowerArm);
	setmatrial_Copper(LeftLowerArm);
	setmatrial_Copper(RightUpperLeg);
	setmatrial_Copper(LeftUpperLeg);
	setmatrial_Copper(RightLowerLeg);

	setmatrial_Copper(LeftLowerLeg);
	setmatrial_Bronze(JIANBING);
	setmatrial_Bronze(JIANGE);
	setmatrial_Bronze(JIANSHEN);
	setmatrial_Bronze(JIANJIAN);


	// 将物体的顶点数据传递
	painter->addMesh(Torso,"torso", "./assets/beidaiku.jpg", vshader, fshader);
	meshList.push_back(Torso);
	painter->addMesh(Head,"head","./assets/enderman_texture.jpg",vshader, fshader);
	meshList.push_back(Head);
	painter->addMesh(RightUpperArm,"right_upper_arm", "./assets/yifu.jpg", vshader, fshader);
	meshList.push_back(RightUpperArm);	
	painter->addMesh(RightLowerArm,"right_lower_arm", "./assets/yifu.jpg", vshader, fshader);
	meshList.push_back(RightLowerArm);
	painter->addMesh(LeftUpperArm,"left_upper_arm", "./assets/yifu.jpg", vshader, fshader);
	meshList.push_back(LeftUpperArm);	
	painter->addMesh(LeftLowerArm,"left_lower_arm", "./assets/yifu.jpg", vshader, fshader);
	meshList.push_back(LeftLowerArm);
	painter->addMesh(RightUpperLeg,"right_upper_leg", "./assets/ku.jpg", vshader, fshader);
	meshList.push_back(RightUpperLeg);	
	painter->addMesh(RightLowerLeg,"right_lower_leg","./assets/ku.jpg", vshader, fshader);
	meshList.push_back(RightLowerLeg);
	painter->addMesh(LeftUpperLeg,"left_upper_leg", "./assets/ku.jpg", vshader, fshader);
	meshList.push_back(LeftUpperLeg);
	painter->addMesh(LeftLowerLeg,"left_lower_leg","./assets/ku.jpg", vshader, fshader);
	meshList.push_back(LeftLowerLeg);
	painter->addMesh(JIANBING,"jianbing", "./assets/jian2.jpg", vshader, fshader);
	meshList.push_back(JIANBING);
	painter->addMesh(JIANGE,"jiange", "./assets/jian2.jpg", vshader, fshader);
	meshList.push_back(JIANGE);
	painter->addMesh(JIANSHEN, "jianshen", "./assets/jian.jpg", vshader, fshader);
	meshList.push_back(JIANSHEN);
	painter->addMesh(JIANJIAN, "jianjian", "./assets/jian1.jpg", vshader, fshader);
	meshList.push_back(JIANJIAN);
	painter->addMesh(Ground, "ground", "./assets/ground.jpg", vshader, fshader);
	meshList.push_back(Ground);

	//
	// 设置头部贴图
	//Head->generateCube(White);
	//painter->addMesh(Head, "head", "C:\\Users\\詹耿羽\\Desktop\\2023193026-詹耿羽-源代码\\assets\\1.jpg", vshader, fshader);
	//meshList.push_back(Head);
	// 初始化风车杆和扇叶
	WindmillPole->generateCylinder(20, 0.1, 1.0);  // 细长杆子
	WindmillBlade->generateCube(White);           // 扇叶

	// 设置风车材质
	setmatrial_Copper(WindmillPole);
	setmatrial_Bronze(WindmillBlade);

	// 添加风车到绘制列表
	painter->addMesh(WindmillPole, "windmill_pole", "", vshader, fshader);
	meshList.push_back(WindmillPole);

	painter->addMesh(WindmillBlade, "windmill_blade", "", vshader, fshader);
	meshList.push_back(WindmillBlade);
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
}



void display()
{	//
	glm::vec3 skyColor(1.0, 1.0, 1.0); // 初始颜色（白色）
	glm::vec3 targetColor(0.5, 0.7, 1.0); // 目标颜色（浅蓝色）
	static float transitionProgress = 0.0f; // 转变进度

	// 更新渐变进度
	transitionProgress += 0.00001f; // 控制过渡速度
	if (transitionProgress > 1.0f) transitionProgress = 1.0f; // 限制进度最大值

	// 线性插值计算当前颜色
	skyColor.r = (1.0f - transitionProgress) * 1.0f + transitionProgress * 0.5f; // 红色分量
	skyColor.g = (1.0f - transitionProgress) * 1.0f + transitionProgress * 0.7f; // 绿色分量
	skyColor.b = (1.0f - transitionProgress) * 1.0f + transitionProgress * 1.0f; // 蓝色分量

	// 设置背景颜色
	glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0);

	//
	if (isClapping) {
		clapProgress += 0.1f; // 控制荡剑速度
		float clapAngle = 20.0f * sin(clapProgress * 10.0f);

		robot.theta[robot.LeftLowerArm].x = clapAngle;
		robot.theta[robot.RightLowerArm].x = -clapAngle;

		if (clapProgress >= M_PI) { // 完成一次荡剑
			isClapping = false;
			robot.theta[robot.LeftLowerArm].x = 0.0f;
			robot.theta[robot.RightLowerArm].x = 0.0f;
		}
	}

	//
	static float previousTime = glfwGetTime();
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - previousTime; // 计算每帧的时间差
	previousTime = currentTime;
	// 更新跳跃状态
	jump(deltaTime);
	//
	/*
	if (isWaveActive) {
		waveRadius += 0.2f; // 冲击波扩展速度
		glm::mat4 waveMatrix = glm::mat4(1.0);
		waveMatrix = glm::scale(waveMatrix, glm::vec3(waveRadius, 0.1, waveRadius));
		painter->drawMesh(waveMatrix, Ground, objects[robot.Ground], light, camera, 0);

		// 冲击波消失条件
		if (waveRadius > 5.0f) {
			isWaveActive = false;
		}
	}
	*/
	//


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 物体的变换矩阵
	glm::mat4 modelMatrix = glm::mat4(1.0);

	// 保持变换矩阵的栈
	MatrixStack mstack;
	//缩小机器人并向下平移
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5,0.5,0.5));
	float offset = -0.5*(robot.TORSO_HEIGHT+robot.UPPER_LEG_HEIGHT+robot.LOWER_LEG_HEIGHT);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0,offset, 0.0));

	//记录行走时的平移量
	modelMatrix = glm::translate(modelMatrix, robot.move_offect);
    // 躯干（这里我们希望机器人的躯干只绕Y轴旋转，所以只计算了RotateY）
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.Torso].y), glm::vec3(0.0, 1.0, 0.0));
	torso(modelMatrix);

	mstack.push(modelMatrix); // 保存躯干变换矩阵
    // 头部（这里我们希望机器人的头部只绕Y轴旋转，所以只计算了RotateY）
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, robot.TORSO_HEIGHT, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.Head].y), glm::vec3(0.0, 1.0, 0.0));
	head(modelMatrix);
	modelMatrix = mstack.pop(); // 恢复躯干变换矩阵


    // =========== 左臂 ===========
	mstack.push(modelMatrix);   // 保存躯干变换矩阵
    // 左大臂（可以绕x,z轴旋转，后面部分也是如此）
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5 * robot.TORSO_WIDTH - 0.5 * robot.UPPER_ARM_WIDTH, robot.TORSO_HEIGHT, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.LeftUpperArm].x), glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.LeftUpperArm].z), glm::vec3(0.0, 0.0, 1.0));
	left_upper_arm(modelMatrix);

    // 左小臂
	modelMatrix = glm::translate(modelMatrix,glm::vec3(0.0, -robot.UPPER_ARM_HEIGHT, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.LeftLowerArm].x), glm::vec3(1.0,0.0,0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.LeftLowerArm].z), glm::vec3(0.0,0.0,1.0));
	left_lower_arm(modelMatrix);
	modelMatrix = mstack.pop();

    // =========== 右臂 及 剑===========

	//  右大臂
	mstack.push(modelMatrix);  
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5 * robot.TORSO_WIDTH + 0.5 * robot.UPPER_ARM_WIDTH, robot.TORSO_HEIGHT, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.RightUpperArm].x), glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.RightUpperArm].z), glm::vec3(0.0, 0.0, 1.0));
	right_upper_arm(modelMatrix);

    //  右小臂
	modelMatrix = glm::translate(modelMatrix,glm::vec3(0.0, -robot.UPPER_ARM_HEIGHT, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.RightLowerArm].x), glm::vec3(1.0,0.0,0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.RightLowerArm].z), glm::vec3(0.0,0.0,1.0));
	right_lower_arm(modelMatrix);

	//剑柄
	modelMatrix = glm::translate(modelMatrix,glm::vec3(0.0,-robot.LOWER_ARM_HEIGHT,0.0) );
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.JIANBING].x), glm::vec3(1.0,0.0,0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0.0,0.0,1.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.JIANBING].z), glm::vec3(0.0,0.0,1.0));
	jianbing(modelMatrix);

	//剑格
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, robot.JIANBING_HIGHT+0.5*robot.JIANGE_HIGHT, 0.0));
	jiange(modelMatrix);

	//剑身
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, 0.5*robot.JIANGE_HIGHT+0.5*robot.JIANSHEN_HIGHT,0.0));
	jianshen(modelMatrix);

	//剑尖
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0,0.5*robot.JIANSHEN_HIGHT,0.0));
	jianjian(modelMatrix);


	modelMatrix = mstack.pop();

    // =========== 左腿 ===========
	
    //  左大腿
	mstack.push(modelMatrix);   
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.25 * robot.TORSO_WIDTH, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.LeftUpperLeg].x), glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.LeftUpperLeg].z), glm::vec3(0.0, 0.0, 1.0));
	left_upper_leg(modelMatrix);

	//  左小腿
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, -robot.UPPER_LEG_HEIGHT, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.LeftLowerLeg].z), glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.LeftLowerLeg].z), glm::vec3(0.0, 0.0, 1.0));
	left_lower_leg(modelMatrix);
	modelMatrix = mstack.pop();


    // =========== 右腿 ===========
	
    //  右大腿
	mstack.push(modelMatrix);  
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.25 * robot.TORSO_WIDTH, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.RightUpperLeg].x), glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.RightUpperLeg].z), glm::vec3(0.0, 0.0, 1.0));
	right_upper_leg(modelMatrix);

    //  右小腿
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, -robot.UPPER_LEG_HEIGHT, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.RightLowerLeg].x), glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(robot.theta[robot.RightLowerLeg].z), glm::vec3(0.0, 0.0, 1.0));
	right_lower_leg(modelMatrix);
	modelMatrix = mstack.pop();

	// ========== 地面 ==============
	ground();
	//
	
	 // 绘制静止机器人
	glm::mat4 staticRobotMatrix = glm::mat4(1.0);
	staticRobotMatrix = glm::scale(staticRobotMatrix, glm::vec3(0.5, 0.5, 0.5));
	float staticOffset = -0.5 * (robot.TORSO_HEIGHT + robot.UPPER_LEG_HEIGHT + robot.LOWER_LEG_HEIGHT);
	staticRobotMatrix = glm::translate(staticRobotMatrix, glm::vec3(-9.0, staticOffset, 0.0)); // 偏移位置以不重叠主机器人

	// 躯干
	glm::mat4 staticModelMatrix = staticRobotMatrix;
	torso(staticModelMatrix);

	// 保存和绘制头部
	MatrixStack staticStack;
	staticStack.push(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(0.0, robot.TORSO_HEIGHT, 0.0));
	head(staticModelMatrix);
	staticModelMatrix = staticStack.pop();

	// 左臂
	staticStack.push(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(-0.5 * robot.TORSO_WIDTH - 0.5 * robot.UPPER_ARM_WIDTH, robot.TORSO_HEIGHT, 0.0));
	left_upper_arm(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(0.0, -robot.UPPER_ARM_HEIGHT, 0.0));
	left_lower_arm(staticModelMatrix);
	staticModelMatrix = staticStack.pop();

	// 右臂
	staticStack.push(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(0.5 * robot.TORSO_WIDTH + 0.5 * robot.UPPER_ARM_WIDTH, robot.TORSO_HEIGHT, 0.0));
	right_upper_arm(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(0.0, -robot.UPPER_ARM_HEIGHT, 0.0));
	right_lower_arm(staticModelMatrix);
	staticModelMatrix = staticStack.pop();

	// 左腿
	staticStack.push(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(-0.25 * robot.TORSO_WIDTH, 0.0, 0.0));
	left_upper_leg(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(0.0, -robot.UPPER_LEG_HEIGHT, 0.0));
	left_lower_leg(staticModelMatrix);
	staticModelMatrix = staticStack.pop();

	// 右腿
	staticStack.push(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(0.25 * robot.TORSO_WIDTH, 0.0, 0.0));
	right_upper_leg(staticModelMatrix);
	staticModelMatrix = glm::translate(staticModelMatrix, glm::vec3(0.0, -robot.UPPER_LEG_HEIGHT, 0.0));
	right_lower_leg(staticModelMatrix);
	staticModelMatrix = staticStack.pop();
	
	//
	// 
	/*
	// 绘制左侧更小的机器人
	glm::mat4 smallRobotMatrix = glm::mat4(1.0);
	smallRobotMatrix = glm::scale(smallRobotMatrix, glm::vec3(0.3, 0.3, 0.3)); // 缩小比例
	float smallOffset = -0.5 * (robot.TORSO_HEIGHT + robot.UPPER_LEG_HEIGHT + robot.LOWER_LEG_HEIGHT);
	smallRobotMatrix = glm::translate(smallRobotMatrix, glm::vec3(-5.0, smallOffset, 0.0)); // 偏移到左侧

	// 躯干
	glm::mat4 smallModelMatrix = smallRobotMatrix;
	torso(smallModelMatrix);

	// 保存和绘制头部
	MatrixStack smallStack;
	smallStack.push(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(0.0, robot.TORSO_HEIGHT, 0.0));
	head(smallModelMatrix);
	smallModelMatrix = smallStack.pop();

	// 左臂
	smallStack.push(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(-0.5 * robot.TORSO_WIDTH - 0.5 * robot.UPPER_ARM_WIDTH, robot.TORSO_HEIGHT, 0.0));
	left_upper_arm(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(0.0, -robot.UPPER_ARM_HEIGHT, 0.0));
	left_lower_arm(smallModelMatrix);
	smallModelMatrix = smallStack.pop();

	// 右臂
	smallStack.push(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(0.5 * robot.TORSO_WIDTH + 0.5 * robot.UPPER_ARM_WIDTH, robot.TORSO_HEIGHT, 0.0));
	right_upper_arm(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(0.0, -robot.UPPER_ARM_HEIGHT, 0.0));
	right_lower_arm(smallModelMatrix);
	smallModelMatrix = smallStack.pop();

	// 左腿
	smallStack.push(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(-0.25 * robot.TORSO_WIDTH, 0.0, 0.0));
	left_upper_leg(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(0.0, -robot.UPPER_LEG_HEIGHT, 0.0));
	left_lower_leg(smallModelMatrix);
	smallModelMatrix = smallStack.pop();

	// 右腿
	smallStack.push(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(0.25 * robot.TORSO_WIDTH, 0.0, 0.0));
	right_upper_leg(smallModelMatrix);
	smallModelMatrix = glm::translate(smallModelMatrix, glm::vec3(0.0, -robot.UPPER_LEG_HEIGHT, 0.0));
	right_lower_leg(smallModelMatrix);
	smallModelMatrix = smallStack.pop();*/
	// 
	// 动态计算扇叶旋转角度
	static float bladeAngle = 0.0f;
	bladeAngle += 0.7f; // 每帧旋转
	if (bladeAngle >= 360.0f) {
		bladeAngle = 0.0f;
	}

	// 风车放置在机器人右侧
	glm::mat4 windmillMatrix = glm::translate(glm::mat4(1.0), glm::vec3(6.0, 0.0, 0.0));
	drawWindmill(windmillMatrix, bladeAngle);

}

void rightgo( int rotate_axis)
{
	int rightleg = robot.RightUpperLeg;
	int leftarm = robot.LeftUpperArm;
	robot.theta[leftarm][rotate_axis] += 15.0;
	robot.theta[rightleg][rotate_axis] += 15.0;
	if(robot.theta[rightleg][rotate_axis]>360)
	{
		robot.theta[rightleg][rotate_axis] -= 360;
	}
	if(robot.theta[leftarm][rotate_axis]>360)
	{
		robot.theta[leftarm][rotate_axis] -= 360;
	}
}
void rightback(int rotate_axis)
{
	int rightleg = robot.RightUpperLeg;
	int leftarm = robot.LeftUpperArm;
	robot.theta[leftarm][rotate_axis] -= 15.0;
	robot.theta[rightleg][rotate_axis] -= 15.0;
	if(robot.theta[rightleg][rotate_axis]>360)
	{
		robot.theta[rightleg][rotate_axis] -= 360;
	}
	if(robot.theta[leftarm][rotate_axis]>360)
	{
		robot.theta[leftarm][rotate_axis] -= 360;
	}

}
void leftgo(int rotate_axis)
{
	int leftleg = robot.LeftUpperLeg;
	int rightarm = robot.RightUpperArm;
	robot.theta[leftleg][rotate_axis] += 15.0;
	robot.theta[rightarm][rotate_axis] += 15.0;
	if(robot.theta[leftleg][rotate_axis] > 360)
	{
		robot.theta[leftleg][rotate_axis] -= 360;
	}
	if(robot.theta[rightarm][rotate_axis] > 360)
	{
		robot.theta[rightarm][rotate_axis] -= 360;
	}
}
void leftback(int rotate_axis)
{
	int leftleg = robot.LeftUpperLeg;
	int rightarm = robot.RightUpperArm;
	robot.theta[leftleg][rotate_axis] -= 15.0;
	robot.theta[rightarm][rotate_axis] -= 15.0;
	if(robot.theta[leftleg][rotate_axis] > 360)
	{
		robot.theta[leftleg][rotate_axis] -= 360;
	}
	if(robot.theta[rightarm][rotate_axis] > 360)
	{
		robot.theta[rightarm][rotate_axis] -= 360;
	}
}

void torso_move(int direction, int axis)	//direction表示向前还是向后,1向前，0向后, axis为前进方向的轴
{
	int tx = robot.move_offect.x;
	int ty = robot.move_offect.y;
	int tz = robot.move_offect.z;
	if(axis == X_AXIS)
	{
		if(direction == 1)
		{
			tx +=1;
			
		}
		else if(direction == 0)
		{
			tx -=1;
		}
	}
	if(axis == Z_AXIS)
	{
		if(direction == 1)
		{
			tz+=1;
		}
		else if(direction == 0)
		{
			tz-=1;
		}
	}
	robot.move_offect = glm::vec3(tx,ty,tz);
}

//行走的第一个动作
void move_one(int direction, int axis)
{
	int rotate_axis=0;	//旋转的轴
	/*if(axis == X_AXIS)
	{
		rotate_axis = Z_AXIS;
	}
	if(axis == Z_AXIS)
	{
		rotate_axis = X_AXIS;
	}*/
	rightgo(rotate_axis);
	leftback(rotate_axis);
	torso_move(direction, axis);
}

//行走的第二个动作
void move_two(int direction,int axis)
{
	int rotate_axis=0;	//旋转的轴
	/*if(axis == X_AXIS)
	{
		rotate_axis = Z_AXIS;
	}
	if(axis == Z_AXIS)
	{
		rotate_axis = X_AXIS;
	}*/
	rightback(rotate_axis);
	leftgo(rotate_axis);
	torso_move(direction, axis);
}

void swordAttack() {

	// 挥剑的动作序列
	robot.theta[robot.RightUpperArm].x += 40.0;
	robot.theta[robot.RightLowerArm].x += 45.0;
	//robot.theta[robot.JIANBING].z += 70.0;

	// 限制角度，避免超过360度
	robot.theta[robot.RightUpperArm].x = fmod(robot.theta[robot.RightUpperArm].x, 360.0);
	robot.theta[robot.RightLowerArm].x = fmod(robot.theta[robot.RightLowerArm].x, 360.0);
	//robot.theta[robot.JIANBING].z = fmod(robot.theta[robot.JIANBING].z, 360.0);
}
void printHelp()
{
	std::cout << "Keyboard Response:" << std::endl;
	std::cout << "ESC: Exit." << std::endl;
	std::cout << "J : right leg out." << std::endl;
	std::cout << "K : left leg out." << std::endl;
	std::cout << "Note : The Enderman moves forward : J and K are entered alternately." << std::endl;
	std::cout << "T / Y : Turn the body." << std::endl;
	std::cout << "W : Set direction of movement to negative Z." << std::endl;
	std::cout << "S : set the direction of movement to positive or negative Z - axis." << std::endl;
	std::cout << "A : Sets the direction of motion to negative X." << std::endl;
	std::cout << "D : Sets motion direction to positive X - axis." << std::endl;
	std::cout << "L : Make the Enderman jump." << std::endl;
	std::cout << "C : Make Enderman wield swords" << std::endl;
	std::cout << "V : Make Enderman swing swords" << std::endl;
	std::cout << "Z/X : Adjust the Enderman's side" << std::endl;
	std::cout << "B : Enderman moves forward instantly" << std::endl;
	std::cout << "N : Enderman moves instantly to the right." << std::endl;
	std::cout << "1 / 2 / 3 / 4 : Control sword rotation" << std::endl;
	std::cout << "SPACE : Reset camera view" << std::endl;
	std::cout << "u / U : Adjust angle left / right" << std::endl;
	std::cout << "i / I : Angle up / down" << std::endl;
	std::cout << "o / O : Controls the camera's distance." << std::endl;

}

//鼠标响应
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		float half_winx = WIDTH / 2.0;
		float half_winy = HEIGHT / 2.0;
		float lx = float(x - half_winx) / half_winx;
		float ly = float(HEIGHT - y - half_winy) / half_winy;

		glm::vec3 pos = light->getTranslation();

		pos.x = lx;
		pos.y = ly+3;

		light->setTranslation(pos);

		// }
	}
}

int direction = 1;
int axis = Z_AXIS;
// 键盘响应函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	
	if (action == GLFW_PRESS) {
		switch (key)
		{
			case GLFW_KEY_1: robot.theta[robot.JIANBING].x += 15;
			if (robot.theta[robot.JIANBING].x > 360.0)
				robot.theta[robot.JIANBING].x -= 360.0;
				 break;
			case GLFW_KEY_2: robot.theta[robot.JIANBING].x -= 15;
			if (robot.theta[robot.JIANBING].x > 360.0)
				robot.theta[robot.JIANBING].x -= 360.0;
				 break;
			case GLFW_KEY_3: robot.theta[robot.JIANBING].z += 15;
			if (robot.theta[robot.JIANBING].z > 360.0)
				robot.theta[robot.JIANBING].z -= 360.0;
				 break;
				case GLFW_KEY_4: robot.theta[robot.JIANBING].z -= 15;
			if (robot.theta[robot.JIANBING].z > 360.0)
				robot.theta[robot.JIANBING].z -= 360.0;
				 break;
			case GLFW_KEY_J:
			move_one(direction, axis);
			break;
			case GLFW_KEY_K:
			move_two(direction,axis);
			break;
			case GLFW_KEY_W:
			direction = 0;
			axis = Z_AXIS;break;
			case GLFW_KEY_A:
			direction = 0;
			axis = X_AXIS;break;
			case GLFW_KEY_S:
			direction = 1;
			axis = Z_AXIS;break;
			case GLFW_KEY_D:
			direction = 1;
			axis = X_AXIS;break;
			case GLFW_KEY_Z: // 左转
				turn(-15.0);
				break;
			case GLFW_KEY_X: // 右转
				turn(15.0);
				break;
			case GLFW_KEY_C: // C键挥剑
				swordAttack();
				break;
			case GLFW_KEY_B: // 按下 B 键进行瞬移
				teleport(5.0f, Z_AXIS); // 在 Z 轴方向瞬移 5 个单位
				break;
			case GLFW_KEY_N: // 按下 N 键进行瞬移
				teleport(5.0f, X_AXIS); // 在 X 轴方向瞬移 5 个单位
				break;
			case GLFW_KEY_T:
			robot.theta[robot.Torso].y += 90;
			if (robot.theta[robot.Torso].y > 360.0)
				robot.theta[robot.Torso].y -= 360.0;
				break;
			case GLFW_KEY_Y:
			robot.theta[robot.Torso].y -= 90;
			if (robot.theta[robot.Torso].y > 360.0)
				robot.theta[robot.Torso].y -= 360.0;
				break;		
			case GLFW_KEY_ESCAPE: exit(EXIT_SUCCESS); break;
			case GLFW_KEY_Q: exit(EXIT_SUCCESS); break;
			case GLFW_KEY_V: // 荡剑
				clap();
				break;

			case GLFW_KEY_L: // 按下 L 键开始跳跃
				if (!isJumping) {
					isJumping = true; // 进入跳跃状态
				}
				break;
			
			default:
				camera->keyboard(key, action, mode);
				break;
		}
	}
}


void cleanData() {
	
	// 释放内存
	delete camera;
	camera = NULL;

	painter->cleanMeshes();

	delete painter;
	painter = NULL;

	for (int i=0; i<meshList.size(); i++) {
		meshList[i]->cleanData();
		delete meshList[i];
	}
	meshList.clear();

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main(int argc, char **argv)
{
	// 初始化GLFW库，必须是应用程序调用的第一个GLFW函数
	glfwInit();

	// 配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 100);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 配置窗口属性
	GLFWwindow* window = glfwCreateWindow(600, 600, "2023193026-Zhan Gengyu-FINALS", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	init();
	objects = painter->getOpenGLObj(); //获得painter中的object数组，方便绘制
	printHelp();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanData();
	return 0;
}