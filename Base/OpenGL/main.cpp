#pragma region 
#include "Angel.h"
typedef vec4 point4;
typedef vec4 color4;
using namespace std;
#pragma endregion Khai báo thư viện và định nghĩa kiểu dữ liệu

#pragma region 
GLuint program;
GLuint model_loc;
GLuint projection_loc;
GLuint view_loc;
#pragma endregion Các biến kết hợp vshader dùng để chạy chương trình, NO LOGIC

#pragma region
mat4 wolrd_matrix;
mat4 projection;
mat4 view;
#pragma endregion Các biến ma trận model, projection, view, symbol

#pragma region
// Số các đỉnh của các tam giác trong hình lập phương
const int soDinhLapPhuong = 36;
// Số các đỉnh của các tam giác trong hình trụ (độ chính xác 8 mặt)
// Mỗi mặt ngang là 1 hcn = 2 tam giác => 6 đỉnh, tổng 8 mặt = 6 * 8
// 2 đáy, mỗi đáy 8 tam giác = 3 * 8 * 2 đáy
// 6 * 8 + 3 * 8 * 2 = 96
const int soDinhHinhTru = 96;
// 4 * 6 * 8 = 192
const int soDinhHinhTron = 192;

// Danh sách các đỉnh của các tam giác cần vẽ
point4 arrDinhTamGiac[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];
// Danh sách các màu tương ứng cho các đỉnh trên
color4 arrMauCacDinh[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];
//Danh sách các vector pháp tuyến ứng với mỗi đỉnh
vec3 arrVecPhapTuyen[soDinhLapPhuong + soDinhHinhTru + soDinhHinhTron];


// Danh sách 8 đỉnh của hình lập phương
point4 cacDinhLapPhuong[8];
// Danh sách các màu cho 8 đỉnh hình lập phương
color4 cacMauHinhLapPhuong[8];

// Danh sách các đỉnh của hình trụ
// 8 cạnh bên, mỗi cạnh 2 đỉnh, 2 tâm tại 2 đáy
// 2 * 8 + 2 = 18
point4 cacDinhHinhTru[18];
// Danh sách các màu cho 10 mặt hình trụ
color4 cacMauHinhTru[10];

// Hình tròn
point4 cacDinhHinhTron[5][8];
color4 cacMauHinhTron[10];
#pragma endregion Các biến tính toán trong bộ nhớ

#pragma region
// Vị trí ánh sáng
point4 viTriAnhSang(0.0, 10.0, 10.0, 1.0);
// Màu gốc ánh sáng
color4 mauAnhSang(1, 1, 1, 1.0);
// Màu khuếch tán
color4 mauAnhSangKhuechTan(1.0, 1.0, 1.0, 1.0);
// Màu gương phản xạ lại
color4 mauAnhSangPhanXaGuong(1.0, 1.0, 1.0, 1.0);

// Màu gốc vật liệu
color4 mauVatLieu;
// Màu khuếch tán vật liệu
color4 mauVatLieuKhuechTan;
// Màu phản xạ gương vật liệu
color4 mauVatLieuPhanXaGuong;
// Độ bóng vật liệu
float doBongVatLieu = 1000.0;

// Màu được trộn từ 2 màu gốc
color4 mauGocTron;
// Màu khuếch tán được trộn từ 2 màu gốc
color4 mauKhuechTanTron;
// Màu phản xạ gương được trộn từ 2 màu gốc
color4 mauPhanXaGuongTron;
#pragma  endregion Các biến ánh sáng, vật liệu

#pragma region 
//GLfloat cam_Eye[3] = { 0, 0, 0 };
//float cam_Rotation[3] = { 90,0,180 };
float cam_Rotation[3] = {90,0,0 };
//GLfloat dr = 5;
vec4 eye(0, 5, 0, 1);
vec4 at(0, 0, 0, 1);
vec4 up(0, 1, 0, 1);

bool che_do_chi_tiet = false;
#pragma endregion Các biến hành động




#pragma region 
// Gán toạ độ điểm và giá trị màu cho HLP
void TinhDiemVaMauLapPhuong()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	cacDinhLapPhuong[0] = point4(-0.5, -0.5, 0.5, 1.0);
	cacDinhLapPhuong[1] = point4(-0.5, 0.5, 0.5, 1.0);
	cacDinhLapPhuong[2] = point4(0.5, 0.5, 0.5, 1.0);
	cacDinhLapPhuong[3] = point4(0.5, -0.5, 0.5, 1.0);
	cacDinhLapPhuong[4] = point4(-0.5, -0.5, -0.5, 1.0);
	cacDinhLapPhuong[5] = point4(-0.5, 0.5, -0.5, 1.0);
	cacDinhLapPhuong[6] = point4(0.5, 0.5, -0.5, 1.0);
	cacDinhLapPhuong[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	cacMauHinhLapPhuong[0] = color4(0.0, 0.0, 0.0, 1.0); // đen
	cacMauHinhLapPhuong[1] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhLapPhuong[2] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhLapPhuong[3] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhLapPhuong[4] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhLapPhuong[5] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhLapPhuong[6] = color4(1.0, 0.5, 0.0, 1.0); // cam
	cacMauHinhLapPhuong[7] = color4(0.0, 1.0, 1.0, 1.0); // xanh lơ
}
// Gán toạ độ điểm và giá trị màu cho hình trụ
void TinhDiemVaMauHinhTru()
{
	float tren = 0.5;
	float duoi = -tren;
	// Tâm trên
	cacDinhHinhTru[0] = point4(0, tren, 0, 1);

	// For để gán cho các đỉnh cạnh bên
	for (int i = 1; i <= 8; i++)
	{
		float banKinh = 0.5;
		float rad = (i - 1) * 45.0 / 360 * 2 * M_PI;
		// Tính x va z
		float x = banKinh * cosf(rad);
		float z = banKinh * sinf(rad);
		cacDinhHinhTru[i * 2 - 1] = point4(x, tren, z, 1);
		cacDinhHinhTru[i * 2] = point4(x, duoi, z, 1);
	}

	// Tâm dưới
	cacDinhHinhTru[17] = point4(0, duoi, 0, 1);

	cacMauHinhTru[0] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTru[1] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTru[2] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTru[3] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTru[4] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhTru[5] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTru[6] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTru[7] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTru[8] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTru[9] = color4(1.0, 0.0, 1.0, 1.0); // tím
}
// Gán toạ độ điểm và giá trị màu cho hình tròn
void TinhDiemVaMauHinhTron()
{
	float x, y, z;
	const float r = 0.5;
	const float cv45torad = 45.0 / 360 * 2 * M_PI;
	for (int i = 0; i <= 4; i++)
	{
		y = -r * cosf(i * cv45torad);
		float bk = r * sinf(i * cv45torad);
		//Tính y
		for (int j = 0; j <= 7; j++)
		{
			// Tính x,z
			x = bk * cosf(j * cv45torad);
			z = -bk * sinf(j * cv45torad);
			cacDinhHinhTron[i][j] = point4(x, y, z, 1);
		}
	}

	cacMauHinhTron[0] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTron[1] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTron[2] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTron[3] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTron[4] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhTron[5] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhTron[6] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhTron[7] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhTron[8] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhTron[9] = color4(1.0, 0.0, 1.0, 1.0); // tím
}

// Start HLP = 0
// Start Hình trụ = 0 + 36
// Start Hình tròn = 36 + 96 = 132
int Index = 0;
// Tạo một mặt hình lập phương (đưa đỉnh, pháp tuyến, màu vào mảng thứ tự cho việc chuyển đến bộ nhớ, )
void TaoMotMatHLP(int a, int b, int c, int d)
{
	vec4 u = cacDinhLapPhuong[b] - cacDinhLapPhuong[a];
	vec4 v = cacDinhLapPhuong[c] - cacDinhLapPhuong[b];
	vec3 normal = normalize(cross(u, v));

	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[a]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[b]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[c]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[a]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[c]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[d]; Index++;
}
// Dùng hàm bên trên để đưa vào mảng
void TaoHinhVuong()  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
{
	TaoMotMatHLP(1, 0, 3, 2);
	TaoMotMatHLP(2, 3, 7, 6);
	TaoMotMatHLP(3, 0, 4, 7);
	TaoMotMatHLP(6, 5, 1, 2);
	TaoMotMatHLP(4, 5, 6, 7);
	TaoMotMatHLP(5, 4, 0, 1);
}


// Hình trụ
void TaoMatBenHTru(int a, int b, int c, int d, int mau)
{
	vec4 u = cacDinhHinhTru[b] - cacDinhHinhTru[a];
	vec4 v = cacDinhHinhTru[c] - cacDinhHinhTru[b];
	vec3 phapTuyen = normalize(cross(u, v));

	// a -> b -> c -> a -> c -> d
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[b]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[d]; Index++;
}

void TaoTamGiacDay(int a, int b, int c, int mau)
{
	vec4 u = cacDinhHinhTru[b] - cacDinhHinhTru[a];
	vec4 v = cacDinhHinhTru[c] - cacDinhHinhTru[b];
	vec3 phapTuyen = normalize(cross(u, v));

	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[a]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[b]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTru[mau]; arrDinhTamGiac[Index] = cacDinhHinhTru[c]; Index++;
}

void TaoHTru()
{
	// Mỗi lệnh 6 điểm, 8 lệnh 48 điểm
	TaoMatBenHTru(2, 1, 3, 4, 0);
	TaoMatBenHTru(4, 3, 5, 6, 1);
	TaoMatBenHTru(6, 5, 7, 8, 2);
	TaoMatBenHTru(8, 7, 9, 10, 3);
	TaoMatBenHTru(10, 9, 11, 12, 4);
	TaoMatBenHTru(12, 11, 13, 14, 5);
	TaoMatBenHTru(14, 13, 15, 16, 6);
	TaoMatBenHTru(16, 15, 1, 2, 7);
	// Kết thúc tại điểm 2

	// Mỗi lệnh 3 điểm, 8 lệnh 24 điểm
	// Bắt đầu tại điểm 2
	TaoTamGiacDay(2, 17, 16, 9);
	TaoTamGiacDay(16, 17, 14, 9);
	TaoTamGiacDay(14, 17, 12, 9);
	TaoTamGiacDay(12, 17, 10, 9);
	TaoTamGiacDay(10, 17, 8, 9);
	TaoTamGiacDay(8, 17, 6, 9);
	TaoTamGiacDay(6, 17, 4, 9);
	TaoTamGiacDay(4, 17, 2, 9);

	// Mỗi lệnh 3 điểm, 8 lệnh 24 điểm
	TaoTamGiacDay(1, 0, 3, 8);
	TaoTamGiacDay(3, 0, 5, 8);
	TaoTamGiacDay(5, 0, 7, 8);
	TaoTamGiacDay(7, 0, 9, 8);
	TaoTamGiacDay(9, 0, 11, 8);
	TaoTamGiacDay(11, 0, 13, 8);
	TaoTamGiacDay(13, 0, 15, 8);
	TaoTamGiacDay(15, 0, 1, 8);

	//Tổng 48 + 24 + 24 = 96 = biến soDinhHinhTru
}


// Hình tròn
void TaoMatBenHTron(int i, int j, int i1, int j1, int mau)
{
	vec3 phapTuyen;
	vec4 u;
	vec4 v;
	if (i == 0)
	{
		u = cacDinhHinhTron[i1][j1] - cacDinhHinhTron[i][j];
		v = cacDinhHinhTron[i1][j] - cacDinhHinhTron[i][j];
		phapTuyen = normalize(cross(u, v));
	}
	else
	{
		u = cacDinhHinhTron[i][j1] - cacDinhHinhTron[i][j];
		v = cacDinhHinhTron[i1][j1] - cacDinhHinhTron[i][j1];
		phapTuyen = normalize(cross(u, v));
	}
	// a -> b -> c -> a -> c -> d
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i][j]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j1]; Index++;
	arrVecPhapTuyen[Index] = phapTuyen; arrMauCacDinh[Index] = cacMauHinhTron[mau]; arrDinhTamGiac[Index] = cacDinhHinhTron[i1][j]; Index++;
}

void TaoHTron()
{
	for (int j = 0; j <= 6; j++)
	{
		for (int i = 0; i <= 3; i++)
		{
			TaoMatBenHTron(i, j, i + 1, j + 1, 4);
		}
	}

	for (int i = 0; i <= 3; i++)
	{
		TaoMatBenHTron(i, 7, i + 1, 0, 3);
	}
}


void GenerateCacHinh(void)
{
	TinhDiemVaMauLapPhuong();
	TaoHinhVuong();

	TinhDiemVaMauHinhTru();
	TaoHTru();

	TinhDiemVaMauHinhTron();
	TaoHTron();
}

#pragma endregion Xử lí Generate các hình

#pragma region
// Hàm đổi _param, ví dụ 255 => 255/255 = 1
float RGBConvert(float _param)
{
	return _param / 255;
}
color4 RGBtoColor(int _R, int _G, int _B)
{
	return color4(RGBConvert(_R), RGBConvert(_G), RGBConvert(_B), 1.0);
}
color4 RGBtoColor(int _R, int _G, int _B, int _A)
{
	return color4(RGBConvert(_R), RGBConvert(_G), RGBConvert(_B), RGBConvert(_A));
}
float DEGtoRAD(float DEG)
{
	return DEG / 360 * 2 * M_PI;
}
void TronMau()
{
	//  sử mô hình blinn - Phong
	mauGocTron = mauAnhSang * mauVatLieu;
	mauKhuechTanTron = mauAnhSangKhuechTan * mauVatLieuKhuechTan;
	mauPhanXaGuongTron = mauAnhSangPhanXaGuong * mauVatLieuPhanXaGuong;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, mauGocTron);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, mauKhuechTanTron);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, mauPhanXaGuongTron);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, viTriAnhSang);
	glUniform1f(glGetUniformLocation(program, "Shininess"), doBongVatLieu);
}
// Xem hình để biết về hàm này
void TaoVatLieu(color4 mauGoc, color4 mauPhanXa, color4 mauPXGuong, float doBong)
{
	// Nhập 3 màu vào và trộn màu cho vật thể
	mauVatLieu = mauGoc;
	mauVatLieuPhanXaGuong = mauPXGuong;
	mauVatLieuKhuechTan = mauPhanXa;
	doBongVatLieu = doBong;
	TronMau();
}
// Hàm vẽ hình lập phương tâm O,  với ma trận _matrixPhanCap * _symbol
void VeHinhLapPhuong(mat4 _symbol, mat4 _matrixPhanCap)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrixPhanCap * _symbol);
	glDrawArrays(GL_TRIANGLES, 0, soDinhLapPhuong);    /*Vẽ các tam giác*/
}
// Hàm vẽ hình trụ tâm O, bán kính đáy 0.5, chiều cao 1 với ma trận _matrix * _symbol
void VeHinhTru(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 36, soDinhHinhTru);    /*Vẽ các tam giác*/
}
// Hàm vẽ hình tròn tâm O, bán kính 0.5 với ma trận _matrix * _symbol
void VeHinhTron(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 132, soDinhHinhTron);    /*Vẽ các tam giác*/
}
#pragma endregion Các hàm tạo sẵn

#pragma region 
void KhoiTaoBoDemGPU(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh) + sizeof(arrVecPhapTuyen), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(arrDinhTamGiac), arrDinhTamGiac);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac), sizeof(arrMauCacDinh), arrMauCacDinh);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh), sizeof(arrVecPhapTuyen), arrVecPhapTuyen);
}

void SetupShader(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrDinhTamGiac)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh)));

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(33.0 / 255, 114.0 / 255, 160.0 / 255, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}
#pragma endregion Tạo GPU buffer và setup shader




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///																											  ///
///        ĐỂ VẼ MỘT HÌNH HÃY TẠO VẬT LIỆU VÀ DÙNG 1 TRONG 3 HÀM VẼ HÌNH, XEM MẪU BÀN TRƯỚC KHI LÀM           ///
///																											  ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////




#pragma region 
// Biến matrix dùng cho mô hình phân cấp
mat4 matrixPhanCapDat;

color4 Dat_vl;
color4 Dat_kt;
color4 Dat_pxg;
mat4 Dat_symbol;

float Dat_DiChuyenX;
float Dat_DiChuyenZ;
float Dat_BanhLan;
float Dat_Xoay = 90;
float Dat_MoCua = 0;
float Dat_XoayBanhXe = 0;
float Dat_RungLaCay = 15;
bool Dat_RungLaCayBenPhai = true;
bool Dat_BatDen = false;
bool Dat_VienGachDuocNghien[15];
vec3 Dat_ViTriCu;
vec3 Dat_ViTriMoi;

// Bánh sau
void Dat_CacBanhSau()
{
	//// Màu xám
	//Dat_vl = RGBtoColor(50, 50, 50);
	//Dat_kt = RGBtoColor(80, 80, 80);
	//Dat_pxg = RGBtoColor(120, 120, 120);
	//TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	//// Trục xoay bánh xe
	//Dat_symbol = Scale(0.2, 0.2, 1.8) * RotateX(90);
	//VeHinhTru(Dat_symbol, matrixPhanCapDat);

	// Màu đen
	Dat_vl = RGBtoColor(10, 10, 10);
	Dat_kt = RGBtoColor(30, 30, 30);
	Dat_pxg = RGBtoColor(30, 30, 30);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	// Bánh xe trái và phải, bán kinh 1m
	Dat_symbol = Scale(2, 2, 0.5) * RotateX(90);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);

	/*Dat_symbol = Translate(0, 0, -0.5) * Scale(2, 2, 0.5) * RotateX(90);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);*/
}

// Bánh trước
void Dat_BanhTruoc()
{
	// Màu xám
	Dat_vl = RGBtoColor(50, 50, 50);
	Dat_kt = RGBtoColor(80, 80, 80);
	Dat_pxg = RGBtoColor(120, 120, 120);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	// Trục
	Dat_symbol = Scale(0.2, 0.2, 1.8) * RotateX(90);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);

	// Màu đen
	Dat_vl = RGBtoColor(10, 10, 10);
	Dat_kt = RGBtoColor(30, 30, 30);
	Dat_pxg = RGBtoColor(30, 30, 30);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	// Cái Lu
	Dat_symbol = Scale(2, 2, 1.3) * RotateX(90);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);
}

// Thân xe
void Dat_ThanXe()
{
	Dat_vl = RGBtoColor(200, 200, 50, 50);
	Dat_kt = RGBtoColor(225, 225, 115);
	Dat_pxg = RGBtoColor(237, 237, 186);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	// Vẽ khối ngồi

	Dat_symbol = Translate(0.25, 1.75, 0.75) * Scale(1.5, 2, 0.02);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(0.25, 1, -0.75) * Scale(1.5, 0.5, 0.02);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(1, 1.75, 0) * Scale(0.02, 2, 1.5);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(-0.5, 1.2, 0) * Scale(0.02, 0.9, 1.5);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(0.25, 2.74, 0) * Scale(1.5, 0.02, 1.5);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(0.25, 0.75, 0) * Scale(1.5, 0.02, 1.5);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	// Vẽ cái gờ cho chỗ ngồi
	Dat_symbol = Translate(-0.5, 2.5, 0) * Scale(0.5, 0.5, 1.5) * RotateX(90);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(2, 0.75, 0.8) * Scale(0.5, 2, 0.1);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(2, 0.75, -0.8) * Scale(0.5, 2, 0.1);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(-1, 0, 0.7) * Scale(3, 0.5, 0.1);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(-1, 0, -0.7) * Scale(3, 0.5, 0.1);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_vl = RGBtoColor(10, 10, 10);
	Dat_kt = RGBtoColor(30, 30, 30);
	Dat_pxg = RGBtoColor(30, 30, 30);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	Dat_symbol = Translate(2, 1.5, 0) * Scale(2, 1, 1.5);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(3, 1.5, 0) * Scale(1, 1, 1.5) * RotateX(90);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(0.25, 0.5, 0.8) * Scale(0.5, 1, 0.1);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(0.25, 0.5, -0.8) * Scale(0.5, 1, 0.1);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(-0.35, 1.5, 0) * RotateZ(-30) * Scale(0.05, 0.2, 0.05);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(-0.278, 1.625, 0) * RotateZ(-30) * Scale(0.3, 0.05, 0.3);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);

	Dat_symbol = Translate(0.394, 1, 0) * Scale(0.5, 0.5, 0.5);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);


	// Vẽ cái đèn

	if (Dat_BatDen)
	{
		Dat_vl = RGBtoColor(255, 10, 10);
		Dat_kt = RGBtoColor(255, 20, 20);
		Dat_pxg = RGBtoColor(255, 40, 40);
		TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 10000);
	}
	else
	{
		Dat_vl = RGBtoColor(97, 97, 97);
		Dat_kt = RGBtoColor(181, 181, 181);
		Dat_pxg = RGBtoColor(181, 181, 181);
		TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 10000);
	}

	Dat_symbol = Translate(-0.55, 1.2, 0.5) * Scale(0.1, 0.2, 0.2) * RotateZ(90);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);
	Dat_symbol = Translate(-0.55, 1.2, -0.5) * Scale(0.1, 0.2, 0.2) * RotateZ(90);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);
}

// Cửa xe
void Dat_CuaXe()
{
	Dat_vl = RGBtoColor(190, 117, 58);
	Dat_kt = RGBtoColor(190, 117, 58);
	Dat_pxg = RGBtoColor(216, 193, 174);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);

	Dat_symbol = Translate(0.75, 0, 0) * Scale(1.5, 1.5, 0.02);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);
}

// Tạo hồ nước trên mặt đất
void Dat_HoNuoc(mat4 _trans)
{
	Dat_vl = RGBtoColor(32, 90, 199);
	Dat_kt = RGBtoColor(106, 147, 222);
	Dat_pxg = RGBtoColor(206, 217, 239);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	Dat_symbol = Translate(0, -0.9, 0) * Scale(10, 0, 10);
	VeHinhTru(Dat_symbol, _trans);
}

// Cột bê tông
void Dat_CotBeTong(mat4 _trans)
{
	Dat_vl = RGBtoColor(144, 144, 144);
	Dat_kt = RGBtoColor(155, 155, 155);
	Dat_pxg = RGBtoColor(144, 144, 144);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	Dat_symbol = Translate(0, -0.5, 0) * Scale(1, 1, 10) * RotateX(90);
	VeHinhTru(Dat_symbol, _trans);
}

// Bộ 3 cột bê tông trên mặt đất
void Dat_Bo3CotBeTong(mat4 _trans)
{
	Dat_vl = RGBtoColor(144, 144, 144);
	Dat_kt = RGBtoColor(155, 155, 155);
	Dat_pxg = RGBtoColor(144, 144, 144);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	Dat_symbol = Translate(0.5, -0.5, 0) * Scale(1, 1, 10) * RotateX(90);
	VeHinhTru(Dat_symbol, _trans);
	Dat_symbol = Translate(-0.5, -0.5, 0) * Scale(1, 1, 10) * RotateX(90);
	VeHinhTru(Dat_symbol, _trans);
	Dat_symbol = Translate(0, 0.2, 0) * Scale(1, 1, 10) * RotateX(90);
	VeHinhTru(Dat_symbol, _trans);
}

// Vẽ cái cây
void Dat_CaiCayTo(mat4 _trans)
{
	// Thân cây
	Dat_vl = RGBtoColor(114, 66, 18);
	Dat_kt = RGBtoColor(124, 76, 29);
	Dat_pxg = RGBtoColor(124, 76, 29);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 10000);
	Dat_symbol = Translate(0, -1, 0) * Scale(2, 8, 2) * Translate(0, 0.5, 0);
	VeHinhTru(Dat_symbol, _trans);
	// Cành
	Dat_symbol = Translate(0, 2, 0) * RotateZ(-45 + Dat_RungLaCay) * Scale(1, 5, 1) * Translate(0, 0.5, 0);
	VeHinhTru(Dat_symbol, _trans);
	Dat_symbol = Translate(0, 1, 0) * RotateZ(45 + Dat_RungLaCay) * Scale(1, 5, 1) * Translate(0, 0.5, 0);
	VeHinhTru(Dat_symbol, _trans);

	// Rễ
	Dat_symbol = Translate(0, -0.8, 0) * RotateY(-11) * RotateZ(100) * Scale(0.5, 3, 0.5) * Translate(0, 0.5, 0);
	VeHinhTru(Dat_symbol, _trans);
	Dat_symbol = Translate(0, -0.7, 0) * RotateY(69) * RotateZ(100) * Scale(0.7, 3, 0.8) * Translate(0, 0.5, 0);
	VeHinhTru(Dat_symbol, _trans);
	Dat_symbol = Translate(0, -0.7, 0) * RotateY(195) * RotateZ(100) * Scale(0.7, 3, 0.4) * Translate(0, 0.5, 0);
	VeHinhTru(Dat_symbol, _trans);

	// Lá cây
	Dat_vl = RGBtoColor(61, 187, 52);
	Dat_kt = RGBtoColor(77, 196, 68);
	Dat_pxg = RGBtoColor(132, 208, 127);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 100);
	Dat_symbol = Translate(0, 10, 0) * Scale(10, 10, 10);
	VeHinhTron(Dat_symbol, _trans);
	Dat_symbol = RotateZ(Dat_RungLaCay) * Translate(5, 7, 0) * RotateZ(34) * RotateX(56) * RotateY(78) * Scale(6, 6, 7);
	VeHinhTron(Dat_symbol, _trans);
	Dat_symbol = RotateZ(Dat_RungLaCay) * Translate(-4, 5, 0) * RotateZ(34) * RotateX(56) * RotateY(78) * Scale(4, 3, 5);
	VeHinhTron(Dat_symbol, _trans);

}

// Vẽ cái sân
void Dat_San()
{
	// Đất
	Dat_vl = RGBtoColor(129, 133, 65);
	Dat_kt = RGBtoColor(30, 30, 30);
	Dat_pxg = RGBtoColor(30, 30, 30);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 1000);
	Dat_symbol = Translate(0, -1, 0) * Scale(100, 0.01, 100);
	VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);

	// Vẽ hồ nước
	Dat_HoNuoc(Translate(10, 0, 10));

	// Vẽ cái cây
	Dat_CaiCayTo(Translate(9, 0, -13) * RotateY(145));
	Dat_CaiCayTo(Translate(-8, 0, 6) * Scale(0.7, 0.7, 0.7));

	// Cot Be tong
	Dat_Bo3CotBeTong(Translate(17, 0, 12));

	// Vẽ vài quả bóng màu đỏ
	Dat_vl = RGBtoColor(170, 24, 24);
	Dat_kt = RGBtoColor(205, 76, 76);
	Dat_pxg = RGBtoColor(224, 145, 145);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 100);
	Dat_symbol = Translate(4, -1 + 0.25, -5) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(Dat_symbol, matrixPhanCapDat);
	Dat_symbol = Translate(-5, -1 + 0.25, -8) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(Dat_symbol, matrixPhanCapDat);
	Dat_symbol = Translate(-5, -1 + 0.25, 8) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(Dat_symbol, matrixPhanCapDat);
	Dat_symbol = Translate(5, -1 + 0.25, 8) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(Dat_symbol, matrixPhanCapDat);

	// Vẽ vài quả bóng, cột thấp màu Cam
	Dat_vl = RGBtoColor(218, 119, 54);
	Dat_kt = RGBtoColor(205, 76, 76);
	Dat_pxg = RGBtoColor(224, 145, 145);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 100);
	Dat_symbol = Translate(14, -1 + 0.25, -9) * Scale(0.5, 0.5, 0.5);
	VeHinhTron(Dat_symbol, matrixPhanCapDat);
	Dat_symbol = Translate(-14, -1 + 0.25, -9) * Scale(0.5, 0.5, 0.5);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);
	Dat_symbol = Translate(-14, -1 + 0.25, 9) * Scale(0.5, 0.5, 0.5);
	VeHinhTru(Dat_symbol, matrixPhanCapDat);
}

// Vẽ viên gạch được nghiền
void Dat_VienGachNghien(int i, float _x, float _z)
{
	Dat_vl = RGBtoColor(25, 25, 25);
	Dat_kt = RGBtoColor(25, 25, 25);
	Dat_pxg = RGBtoColor(25, 25, 25);
	TaoVatLieu(Dat_vl, Dat_kt, Dat_pxg, 100);

	if (Dat_DiChuyenX <= _x + 1.5 && Dat_DiChuyenX >= _x - 1.5 && Dat_DiChuyenZ >= _z - 1.5 && Dat_DiChuyenZ <= _z + 1.5)
	{
		Dat_VienGachDuocNghien[i] = true;
	}

	if (!Dat_VienGachDuocNghien[i])
	{
		Dat_symbol = Translate(_x, 0, _z) * Scale(1.5, 0.5, 1.5);
		VeHinhTron(Dat_symbol, matrixPhanCapDat);
	}
	else
	{
		Dat_symbol = Translate(_x, -1.2, _z) * Scale(2, 0.5, 2);
		VeHinhLapPhuong(Dat_symbol, matrixPhanCapDat);
	}
}

// Main Đạt
void TrinhTienDat()
{
	matrixPhanCapDat = mat4();
	Dat_San();
	for (int i = 0; i < sizeof(Dat_VienGachDuocNghien); i++)
	{
		Dat_VienGachNghien(i, 11 - i * 2, -10);
	}
	matrixPhanCapDat = Translate(Dat_DiChuyenX, 0, Dat_DiChuyenZ) * RotateY(Dat_Xoay + 180);
	Dat_ThanXe();
	mat4 Dat_M1 = matrixPhanCapDat;
	matrixPhanCapDat = Dat_M1 * Translate(-0.5, 2, -0.75) * RotateY(Dat_MoCua);
	Dat_CuaXe();
	matrixPhanCapDat = Dat_M1 * Translate(2, 0, 0.5) * RotateY(Dat_XoayBanhXe) * RotateZ(Dat_BanhLan / 2 / M_PI * 360);
	Dat_CacBanhSau();
	matrixPhanCapDat = Dat_M1 * Translate(2, 0, -0.5) * RotateY(Dat_XoayBanhXe) * RotateZ(Dat_BanhLan / 2 / M_PI * 360);
	Dat_CacBanhSau();
	matrixPhanCapDat = Dat_M1 * Translate(-2.5, 0, 0) * RotateZ(Dat_BanhLan / 2 / M_PI * 360);
	Dat_BanhTruoc();
}
#pragma endregion Trịnh Tiến Đạt

#pragma region 
// Biến matrix dùng cho mô hình phân cấp
mat4 matrixPhanCapDang;

void VuQuangDang()
{
	matrixPhanCapDang = mat4();
}
#pragma endregion Vũ Quang Đáng

#pragma region 
// Biến matrix dùng cho mô hình phân cấp
mat4 matrixPhanCapThuy;
color4 Thuy_vl, Thuy_kt, Thuy_pxg;
mat4 Thuy_symbol;

mat4 vt_nang_tramsform;
mat4 vt_nang_matrixPhanCap;
mat4 Thuy_vitri_vt_nang;

float Thuy_thanhNgang_cao = 31;
float Thuy_quayThanhNgang = 0;
float Thuy_dc_ConLan = -8;
float Thuy_conMoc = 0;

bool Thuy_check_moc[] = { false,false, false }; // 0: đã di chuyển r, 1 vào móc hay chưa

void Thuy_lapPhuong_tru(mat4 _Transform, mat4 matrixPhanCap) {
	// _Transform mã trận của cái lập phương

	Thuy_vl = RGBtoColor(203, 144, 30);
	Thuy_kt = RGBtoColor(223, 164, 50);
	Thuy_pxg = RGBtoColor(243, 184, 70);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);

	Thuy_symbol = _Transform * Translate(1, 1, 1) * Scale(0.1, 2, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(1, 1, -1) * Scale(0.1, 2, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 1, -1) * Scale(0.1, 2, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 1, 1) * Scale(0.1, 2, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0, 2, 1) * Scale(2, 0.1, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0, 2, -1) * Scale(2, 0.1, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(1, 2, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 2, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);


	Thuy_symbol = _Transform * Translate(0, 1, 1) * RotateZ(45) * Scale(0.1, 2 * sqrt(2), 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0, 1, -1) * RotateZ(-45) * Scale(0.1, 2 * sqrt(2), 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 1, 0) * RotateX(-45) * Scale(0.1, 2 * sqrt(2), 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(1, 1, 0) * RotateX(45) * Scale(0.1, 2 * sqrt(2), 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(1, 1, 1) * Scale(0.1, 2, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
}


void Thuy_de(mat4 matrixPhanCap) {
	
	// tạo đế của đối tượng
	Thuy_vl = RGBtoColor(44, 43, 41);
	Thuy_kt = RGBtoColor(64, 63, 61);
	Thuy_pxg = RGBtoColor(84, 83, 81);

	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);

	Thuy_symbol =  Translate(0,-0.5,0) * Scale(5, 1.5, 5);
	VeHinhTru(Thuy_symbol, matrixPhanCap);

}

void Thuy_vatTheNang(mat4 _Transform, mat4 matrixPhanCap) {

	Thuy_vl = RGBtoColor(125, 125, 127);
	Thuy_kt = RGBtoColor(165, 165, 267);
	Thuy_pxg = RGBtoColor(185, 185, 187);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);
	Thuy_symbol = _Transform * Translate(12, 0, 0) * Scale(2, 2, 2.6);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

}

void Thuy_conLan(mat4 _Transform, mat4 matrixPhanCap) {

	Thuy_vl = RGBtoColor(203, 144, 30);
	Thuy_kt = RGBtoColor(223, 164, 50);
	Thuy_pxg = RGBtoColor(243, 184, 70);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 50);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan, 1, 0) * Scale(0.6, 0.5, 2.2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan - 1.5, 1, 0) * Scale(0.6, 0.5, 2.2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

}

void Thuy_moc(mat4 _Transform, mat4 matrixPhanCap) {
	Thuy_vl = RGBtoColor(44, 43, 41);
	Thuy_kt = RGBtoColor(64, 63, 61);
	Thuy_pxg = RGBtoColor(84, 83, 81);

	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);
	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan - 0.8, 0, 0) * Scale(2.6, 0.6, 0.6);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan - 0.6, -0.5, 0) * Scale(0.2, 0.8, 0.2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan - 0.6 + 0.4, -0.8, 0) * Scale(0.6, 0.2, 0.2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan, -1.2, 0) * Scale(0.2, 0.6, 0.2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan - 0.3, -1.4, 0) * Scale(0.6, 0.2, 0.2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan - 0.5, -1.3, 0) * Scale(0.2, 0.3, 0.2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	// day co dinh
	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan, 0, 0) * Scale(0.1, 1.5, 0.1) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan - 1.5 , 0, 0) * Scale(0.1, 1.5, 0.1) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	//cout<< _Transform[0]<<" "<< _Transform[1]<<" "<<_Transform[2]<<" "<< matrixPhanCap[0]<< " " << matrixPhanCap[1]<< " " << matrixPhanCap[2]<<"\n";
	mat4 Thuy_vitri_moc = _Transform * matrixPhanCap * Translate(Thuy_dc_ConLan, -1, 0);
	//cout << matrixPhanCap[0] << " " << matrixPhanCap[1] << " " << _Transform[2] << "\n";
	float Thuy_vung_hut = 2;

	if (
		Thuy_vitri_moc[0][3] >= Thuy_vitri_vt_nang[0][3] - Thuy_vung_hut && Thuy_vitri_moc[0][3] <= Thuy_vitri_vt_nang[0][3] + Thuy_vung_hut &&
		Thuy_vitri_moc[1][3] >= Thuy_vitri_vt_nang[1][3] - Thuy_vung_hut && Thuy_vitri_moc[1][3] <= Thuy_vitri_vt_nang[1][3] + Thuy_vung_hut &&
		Thuy_vitri_moc[2][3] >= Thuy_vitri_vt_nang[2][3] - Thuy_vung_hut && Thuy_vitri_moc[2][3] <= Thuy_vitri_vt_nang[2][3] + Thuy_vung_hut 
		) {
		Thuy_check_moc[2] = true;
	}


	if (Thuy_check_moc[1] && Thuy_check_moc[2]) {
	//if (Thuy_check_moc[1]) {
		vt_nang_matrixPhanCap = matrixPhanCap;
		vt_nang_tramsform = _Transform* Translate(Thuy_dc_ConLan, -1, 0);
		Thuy_check_moc[0] = true;

	}

}

void Thuy_dayConMoc(mat4 _Transform, mat4 matrixPhanCap) {

	 // dây móc nối 
	Thuy_vl = RGBtoColor(44, 43, 41);
	Thuy_kt = RGBtoColor(64, 63, 61);
	Thuy_pxg = RGBtoColor(84, 83, 81);

	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);
	// dây
	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan , -Thuy_conMoc, 0) * Scale(0.1, 0.5 + Thuy_conMoc, 0.1) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(Thuy_dc_ConLan -1.5, -Thuy_conMoc, 0) * Scale(0.1, 0.5 + Thuy_conMoc, 0.1) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
}

void Thuy_tuDien_ngang(mat4 _Transform, mat4 matrixPhanCap) {
	Thuy_vl = RGBtoColor(203, 144, 30);
	Thuy_kt = RGBtoColor(223, 164, 50);
	Thuy_pxg = RGBtoColor(243, 184, 70);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);

	Thuy_symbol = _Transform * Translate(0, 1, -1) * Scale(2, 0.1, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0, 1, 1) * Scale(2, 0.1, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0, 2, 0) * Scale(2, 0.1, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(1, 1.5, -0.5) * RotateX(45) * Scale(0.1, 1.55, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(1, 1.5, 0.5) * RotateX(-45) * Scale(0.1, 1.55, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(1, 1, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 1.5, -0.5) * RotateX(45) * Scale(0.1, 1.55, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 1.5, 0.5) * RotateX(-45) * Scale(0.1, 1.55, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 1, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-0.5, 1.5, 0.4) * RotateZ(-45) * RotateX(-35) * Scale(0.05, 1.6, 0.05);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0.5, 1.5, 0.4) * RotateZ(45) * RotateX(-35) * Scale(0.05, 1.6, 0.05);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0.5, 1.5, -0.45) * RotateZ(45) * RotateX(35) * Scale(0.05, 1.6, 0.05);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-0.5, 1.5, -0.45) * RotateZ(-45) * RotateX(35) * Scale(0.05, 1.6, 0.05);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
}

void Thuy_TB_than(mat4 _Transform, mat4 matrixPhanCap) {
	Thuy_vl = RGBtoColor(125, 125, 127);
	Thuy_kt = RGBtoColor(165, 165, 267);
	Thuy_pxg = RGBtoColor(185, 185, 187);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 50);

	Thuy_symbol = _Transform *  Scale(2.3, 2.2, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 0, -0.9) * RotateX(90) * Scale(2.1, 0.2, 2.1);
	VeHinhTru(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(-1, 0, 0.9) * RotateX(90) * Scale(2.1, 0.2, 2.1);
	VeHinhTru(Thuy_symbol, matrixPhanCap);
}

void Thuy_TB_Kinh(mat4 _Transform, mat4 matrixPhanCap) {
	Thuy_vl = RGBtoColor(10, 10, 10);
	Thuy_kt = RGBtoColor(30, 30, 30);
	Thuy_pxg = RGBtoColor(30, 30, 30);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 50);
	Thuy_symbol = _Transform * Translate(-1, 0, 0) * RotateX(90) * Scale(2.1, 1.5, 2.1);
	VeHinhTru(Thuy_symbol, matrixPhanCap);

}

void Thuy_deDung(mat4 _Transform, mat4 matrixPhanCap) {
	// 1 ô vuông đế đứng
	Thuy_vl = RGBtoColor(203, 144, 30);
	Thuy_kt = RGBtoColor(223, 164, 50);
	Thuy_pxg = RGBtoColor(243, 184, 70);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);

	Thuy_symbol = _Transform * Translate(0, 0, 1) * Scale(2, 0.1, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(0, 0, -1) * Scale(2, 0.1, 0.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(1, 0, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, 0, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform *RotateY(45) * Scale(0.1, 0.1, 2*sqrt(2));
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * RotateY(-45) * Scale(0.1, 0.1, 2 * sqrt(2));
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);


}

void Thuy_LanCan(mat4 _Transform, mat4 matrixPhanCap) {
	// 1 ô để vịn tay
	Thuy_vl = RGBtoColor(203, 144, 30);
	Thuy_kt = RGBtoColor(223, 164, 50);
	Thuy_pxg = RGBtoColor(243, 184, 70);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);

	Thuy_symbol = _Transform * Translate(-1, 0, -1) * Scale(0.1, 1.5, 0.1)*Translate(0, 0.5, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, 0, 1) * Scale(0.1, 1.5, 0.1) * Translate(0, 0.5, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, 1.5, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, 0.5, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, 1, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, 0, 0) * Scale(0.1, 0.1, 2);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
}

void Thuy_CaBin(mat4 _Transform, mat4 matrixPhanCap) {
	Thuy_TB_than(_Transform, matrixPhanCap);
	Thuy_TB_Kinh(_Transform, matrixPhanCap);

	Thuy_deDung(_Transform*Translate(0,-1,2), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(0, -1, 2), matrixPhanCap);

	Thuy_deDung(_Transform * Translate(0, -1, 4), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(0, -1, 4), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(0, -1, 4)*RotateY(90), matrixPhanCap);

	Thuy_deDung(_Transform * Translate(2, -1, 4), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(2, -1, 4) * RotateY(90), matrixPhanCap);

	Thuy_deDung(_Transform * Translate(4, -1, 4), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(4, -1, 4) * RotateY(90), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(6, -1, 4), matrixPhanCap);

	Thuy_deDung(_Transform * Translate(4, -1, 2), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(6, -1, 2), matrixPhanCap);

	Thuy_deDung(_Transform * Translate(4, -1, 0), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(6, -1, 0), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(4, -1, -2) * RotateY(90), matrixPhanCap);

	Thuy_deDung(_Transform * Translate(2, -1, 0), matrixPhanCap);
	Thuy_LanCan(_Transform * Translate(2, -1, -2) * RotateY(90), matrixPhanCap);

}

void Thuy_DeQuay(mat4 _Transform, mat4 matrixPhanCap) {
	Thuy_vl = RGBtoColor(203, 144, 30);
	Thuy_kt = RGBtoColor(223, 164, 50);
	Thuy_pxg = RGBtoColor(243, 184, 70);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);

	Thuy_symbol = _Transform* Translate(0, -1, 0) * Scale(3, 0.3, 3);
	VeHinhTru(Thuy_symbol, matrixPhanCap);
}
void Thuy_Chop(mat4 _Transform, mat4 matrixPhanCap) {
	// vẽ hình chóp lên đỉnh
	Thuy_vl = RGBtoColor(203, 144, 30);
	Thuy_kt = RGBtoColor(223, 164, 50);
	Thuy_pxg = RGBtoColor(243, 184, 70);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);

	Thuy_symbol = _Transform * Translate(1, -1, 1) * RotateZ(9.462322)  * Scale(0.1, sqrt(37), 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, -1, 1) * RotateZ(-9.462322) * Scale(0.1, sqrt(37), 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(1, -1, 1) * RotateZ(40) * Scale(0.1, 2.6, 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-0.65, 1, 1)* RotateZ(-30) * Scale(0.1, 2.1, 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, -1, 1) * RotateZ(-40) * Scale(0.1, 2.6, 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(0.65, 1, 1) * RotateZ(30) * Scale(0.1, 2.1, 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(1, -1, -1) * RotateZ(9.462322) * Scale(0.1, sqrt(37), 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, -1, -1) * RotateZ(-9.462322) * Scale(0.1, sqrt(37), 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(1, -1, -1) * RotateZ(40) * Scale(0.1, 2.6, 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-0.65, 1, -1) * RotateZ(-30) * Scale(0.1, 2.1, 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(-1, -1, -1) * RotateZ(-40) * Scale(0.1, 2.6, 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
	Thuy_symbol = _Transform * Translate(0.65, 1, -1) * RotateZ(30) * Scale(0.1, 2.1, 0.1) * Translate(0.0, 0.5, 0.0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0,5,0)  * Scale(0.3, 0.3, 2.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0, 2.1, 0) * Scale(0.3, 0.3, 2.1);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	// daay caan bang
	Thuy_vl = RGBtoColor(44, 43, 41);
	Thuy_kt = RGBtoColor(64, 63, 61);
	Thuy_pxg = RGBtoColor(84, 83, 81);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);


	Thuy_symbol = _Transform * Translate(0, 5, 0)*RotateZ(12) * Scale(33.5, 0.1, 0.1) * Translate(-0.5, 0, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0, 5, 0) * RotateZ(25) * Scale(16.8, 0.1, 0.1) * Translate(-0.5, 0, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);

	Thuy_symbol = _Transform * Translate(0, 5, 0) * RotateZ(-28) * Scale(15, 0.1, 0.1) * Translate(0.5, 0, 0);
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);
}
void Thuy_vt_nang(mat4 _Transform, mat4 matrixPhanCap)
{


	Thuy_vl = RGBtoColor(78, 53, 38);
	Thuy_kt = RGBtoColor(98, 73, 51);
	Thuy_pxg = RGBtoColor(118, 93, 71);
	TaoVatLieu(Thuy_vl, Thuy_kt, Thuy_pxg, 100);

	Thuy_symbol = _Transform * Scale(5, 2, 2);
	Thuy_vitri_vt_nang = _Transform * matrixPhanCap;

	cout << Thuy_vitri_vt_nang[0] << " " << Thuy_vitri_vt_nang[1] << " " << Thuy_vitri_vt_nang[2] << "\n";
	VeHinhLapPhuong(Thuy_symbol, matrixPhanCap);


}

void Thuy()
{
	matrixPhanCapThuy = Translate(25, 0, -25); // vị trí để project

	// logic cẩu vật thể khởi tạo
	if (!Thuy_check_moc[0]) {
		vt_nang_matrixPhanCap = matrixPhanCapThuy;
		vt_nang_tramsform = Translate(-30, 0, 0);
	}


	Thuy_de(matrixPhanCapThuy);

	for (int i = 0; i <= 14; i++) {

		Thuy_lapPhuong_tru(Translate(0, i * 2, 0), matrixPhanCapThuy);
	}


	mat4 matrixPhanCapThuy_c1 = matrixPhanCapThuy;


	Thuy_CaBin(Translate(-2, Thuy_thanhNgang_cao, -2), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));
	Thuy_DeQuay(Translate(0, Thuy_thanhNgang_cao, 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));

	// phần sau cột
	for (int i = 0; i <= 6; i++)
		Thuy_tuDien_ngang(Translate(i * 2, Thuy_thanhNgang_cao, 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));

	Thuy_vatTheNang(Translate(0, Thuy_thanhNgang_cao, 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));
	// phần trước cột 
	for (int i = 0; i <= 16; i++)
		Thuy_tuDien_ngang(Translate(-i * 2, Thuy_thanhNgang_cao, 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));

	// phần trên xoay
	for (int i = 15; i <= 16; i++) {

		Thuy_lapPhuong_tru(Translate(0, i * 2, 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));
	}
	Thuy_Chop(Translate(0, 18 * 2 - 1, 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));

	Thuy_conLan(Translate(0, Thuy_thanhNgang_cao, 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));
	// Ngoắc
	Thuy_moc(Translate(0, Thuy_thanhNgang_cao + Thuy_conMoc, 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));
	Thuy_dayConMoc(Translate(0, Thuy_thanhNgang_cao + Thuy_conMoc + 1 , 0), matrixPhanCapThuy_c1 * RotateY(Thuy_quayThanhNgang));

	Thuy_vt_nang(vt_nang_tramsform, vt_nang_matrixPhanCap);


}
#pragma endregion Thuỳ

#pragma region 
// Biến matrix dùng cho mô hình phân cấp
mat4 matrixPhanCapPhuong;



void Phuong()
{
	matrixPhanCapPhuong = mat4();
}
#pragma endregion Phượng

#pragma region 
// Biến matrix dùng cho mô hình phân cấp
mat4 matrixPhanCapLinh;




void Linh()
{
	matrixPhanCapLinh = mat4();
}
#pragma endregion Linh

// Mẫu đây nhé mn, ấn dấu cộng để mở nó ra
#pragma region
// Biến matrix dùng cho mô hình phân cấp
mat4 matrixPhanCapBan;

void MatBan()
{
	// Ví dụ cái mặt bàn màu xanh lá RGB: 77, 197, 79
	// Phải có .0
	color4 vl = RGBtoColor(77, 197, 79);
	// Màu khuếch tán sẽ sáng hơn 1 chút RGB: 146,216,155
	color4 kt = RGBtoColor(146, 216, 155);
	// Màu phản xạ gương sáng hơn 1 chút nữa
	color4 pxg = RGBtoColor(200, 229, 201);

	// Gọi hàm tạo vật liệu
	// Biến cuối cùng mà vật liệu càng bóng thì số càng nhỏ
	TaoVatLieu(vl, kt, pxg, 1000);

	// 1 đơn vị là 1 mét
	// x là trục bên phải, y là trục lên trần nhà, z là trục bắn ra màn hình
	// Tạo symbol cho mặt bàn ngang (x) 1m, dày (y) 0.05m, rộng (z) 0.5m
	mat4 symbol = Scale(1, 0.05, 0.5);
	VeHinhLapPhuong(symbol, matrixPhanCapBan);
}

void QuaBong()
{
	color4 vl = RGBtoColor(182, 30, 30);
	color4 kt = RGBtoColor(208, 73, 73);
	color4 pxg = RGBtoColor(233, 133, 133);
	TaoVatLieu(vl, kt, pxg, 1000);
	mat4 symbol = Scale(0.2, 0.2, 0.2);
	VeHinhTron(symbol, matrixPhanCapBan);
}

void MauBan()
{
	// M = I
	matrixPhanCapBan = mat4();
	MatBan();
	// Quả bóng dịch x,y
	matrixPhanCapBan = matrixPhanCapBan * Translate(-0.25, 0.1 + 0.025, 0);
	QuaBong();
}
#pragma endregion Mẫu bàn



#pragma region 
void DisplayModels(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//wolrd_matrix = RotateY(cam_Eye[1]);

	//MauBan();
	TrinhTienDat(); 
	//VuQuangDang();
	Thuy();
	//Linh();
	//Phuong();

	//eye = vec4(cam_Eye[0], cam_Eye[1], cam_Eye[2], 1);
	at = vec4(cosf(DEGtoRAD(cam_Rotation[0])), 0, cosf(DEGtoRAD(cam_Rotation[2])), 0) + eye;
	view = RotateX(cam_Rotation[1]) * LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	projection = Frustum(-0.01, 0.01, -0.01, 0.01, 0.02, 50);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	glViewport(0, 0, 1000, 1000);
	glutPostRedisplay();
	glutSwapBuffers();
}

//void CameraControl(int width, int height)
//{
//	view = LookAt(eye, at, up);
//glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
//
//projection = Frustum(-1, 1, -1, 1, 1, 4);
//glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
//
//glViewport(0, 0, width, height);
//}

void KeyboardControl(unsigned char key, int x, int y)
{
	vec3 cam_forward = normalize(vec3(cosf(DEGtoRAD(cam_Rotation[0])) * abs(cosf(DEGtoRAD(cam_Rotation[1]))),
		-sinf(DEGtoRAD(cam_Rotation[1])),
		cosf(DEGtoRAD(cam_Rotation[2])) * abs(cosf(DEGtoRAD(cam_Rotation[1])))));
	vec3 cam_right = normalize(cross(cam_forward, up));
	vec4 cam_up = normalize(cross(cam_right, cam_forward));
	switch (key) {
	case 033:
		exit(1);
		break;
	case 'f':
		cam_Rotation[0] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		cam_Rotation[2] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'h':
		cam_Rotation[0] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		cam_Rotation[2] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 't':
		cam_Rotation[1] -= 10 * (che_do_chi_tiet ? 0.1 : 1);
		if (cam_Rotation[1] <= -90) cam_Rotation[1] = -90;
		break;
	case 'g':
		cam_Rotation[1] += 10 * (che_do_chi_tiet ? 0.1 : 1);
		if (cam_Rotation[1] >= 90) cam_Rotation[1] = 90;
		break;
	case 'w':
		eye = eye + cam_forward * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 's':
		eye = eye - cam_forward * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'a':
		eye = eye - cam_right * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'd':
		eye = eye + cam_right * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'q':
		eye = eye - cam_up * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'e':
		eye = eye + cam_up * (che_do_chi_tiet ? 0.1 : 1);
		break;
	case 'r':
		che_do_chi_tiet = !che_do_chi_tiet;
		break;
	case 'i':
		Dat_BanhLan += 0.1;
		// Xe di tiến
		Dat_DiChuyenX += cosf(DEGtoRAD(Dat_Xoay)) * 0.1;
		Dat_DiChuyenZ += -sinf(DEGtoRAD(Dat_Xoay)) * 0.1;
		Dat_Xoay -= Dat_XoayBanhXe / 30 / 1.5;
		break;
	case 'k':
		Dat_BanhLan -= 0.1;
		// Xe di lùi
		Dat_DiChuyenX -= cosf(DEGtoRAD(Dat_Xoay)) * 0.1;
		Dat_DiChuyenZ -= -sinf(DEGtoRAD(Dat_Xoay)) * 0.1;
		Dat_Xoay += Dat_XoayBanhXe / 30 / 1.5;
		break;
	case 'j':
		//Dat_Xoay += 5;
		Dat_XoayBanhXe -= 5;
		if (Dat_XoayBanhXe < -30) Dat_XoayBanhXe = -30;
		break;
	case 'l':
		//Dat_Xoay -= 5;
		Dat_XoayBanhXe += 5;
		if (Dat_XoayBanhXe > 30) Dat_XoayBanhXe = 30;
		break;
	case 'm':
		Dat_MoCua += 5;
		if (Dat_MoCua >= 90)
		{
			Dat_MoCua = 90;
		}
		break;
	case 'M':
		Dat_MoCua -= 5;
		if (Dat_MoCua <= 0)
		{
			Dat_MoCua = 0;
		}
		break;
	case 'p':
		Dat_BatDen = !Dat_BatDen;
		break;
		// phần thùy
	case '9':
		Thuy_thanhNgang_cao += 0.1;
		break;
	case '0':
		Thuy_thanhNgang_cao -= 0.1;
		break;
	case '-':
		Thuy_quayThanhNgang += 1;
		break;
	case '=':
		Thuy_quayThanhNgang -= 1;
		break;
	case '[':
		Thuy_dc_ConLan -= 0.2;
		break;
	case ']':
		Thuy_dc_ConLan += 0.2;
		break;
	case ',':
		Thuy_conMoc -= 0.2;
		break;
	case '.':
		Thuy_conMoc += 0.2;
		break;
	case 'o':
		if (Thuy_check_moc[2])
		{
			Thuy_check_moc[1] = !Thuy_check_moc[1];
		}
		break;
	}
}

void IdleControl()
{
	if (Dat_RungLaCayBenPhai)
	{
		Dat_RungLaCay += 0.01;
		if (Dat_RungLaCay >= 1) Dat_RungLaCayBenPhai = !Dat_RungLaCayBenPhai;
	}
	else
	{
		Dat_RungLaCay -= 0.01;
		if (Dat_RungLaCay <= -1) Dat_RungLaCayBenPhai = !Dat_RungLaCayBenPhai;
	}
}
#pragma endregion Các hàm hiển thị model, camera, bàn phím.

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("A Cube is rotated by KeyboardControl and shaded");


	glewInit();

	GenerateCacHinh();
	KhoiTaoBoDemGPU();
	SetupShader();

	glutDisplayFunc(DisplayModels);
	glutKeyboardFunc(KeyboardControl);
	glutIdleFunc(IdleControl);
	//glutReshapeFunc(CameraControl);

	glutMainLoop();
	return 0;
}