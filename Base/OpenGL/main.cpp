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
GLfloat theta[3] = { 0, 0, 0 };
GLfloat dr = 5;
vec4 eye(0, 0, 5, 1);
vec4 at(0, 0, 0, 1);
vec4 up(0, 1, 0, 1);
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
void TronMau()
{
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
	glDrawArrays(GL_TRIANGLES, 0, soDinhLapPhuong);    
}
// Hàm vẽ hình trụ tâm O, bán kính đáy 0.5, chiều cao 1 với ma trận _matrix * _symbol
void VeHinhTru(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 36, soDinhHinhTru);   
}
// Hàm vẽ hình tròn tâm O, bán kính 0.5 với ma trận _matrix * _symbol
void VeHinhTron(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, wolrd_matrix * _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 132, soDinhHinhTron);  
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
	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
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


void TrinhTienDat()
{
	matrixPhanCapDat = mat4();
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



void Thuy()
{
	matrixPhanCapThuy = mat4();
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


	//MauBan();
	//TrinhTienDat();
	//VuQuangDang();
	Thuy();
	//Linh();
	//Phuong();


	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	projection = Frustum(-0.5, 0.5, -0.5, 0.5, 1, 50);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	glViewport(0, 0, 640, 640);
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
	switch (key) {
	case 033:
		exit(1);
		break;
	case 'x':
		wolrd_matrix *= RotateX(dr);
		glutPostRedisplay();
		break;
	case 'X':
		wolrd_matrix *= RotateX(-dr);
		glutPostRedisplay();
		break;
	case 'y':
		wolrd_matrix *= RotateY(dr);
		glutPostRedisplay();
		break;
	case 'Y':
		wolrd_matrix *= RotateY(-dr);
		glutPostRedisplay();
		break;
	case 'z':
		wolrd_matrix *= RotateZ(dr);
		glutPostRedisplay();
		break;
	case 'Z':
		wolrd_matrix *= RotateZ(-dr);
		glutPostRedisplay();
		break;
	case 'a':
		eye.z -= 0.1;
		glutPostRedisplay();
		break;
	case 'A':
		eye.z += 0.1;
		glutPostRedisplay();
		break;
	}
}
#pragma endregion Các hàm hiển thị model, camera, bàn phím.

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("A Cube is rotated by KeyboardControl and shaded");


	glewInit();

	GenerateCacHinh();
	KhoiTaoBoDemGPU();
	SetupShader();

	glutDisplayFunc(DisplayModels);
	glutKeyboardFunc(KeyboardControl);
	//glutReshapeFunc(CameraControl);

	glutMainLoop();
	return 0;
}