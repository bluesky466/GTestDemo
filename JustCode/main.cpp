#include "TwoDimensionalMark.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <vector>

using namespace std;

struct MarkSize{
	MarkSize(int r, int c): row(r),col(c){}
	int row, col;
};
class TestWithMarkSize : public testing::TestWithParam<MarkSize>{
public:
	struct Coord{
		int x, y;
	};

	vector<vector<bool> > CreateContrast(int row, int col, bool flag){
		vector<vector<bool> > contrast(row);
		for (int i = 0; i < row; i++){
			contrast[i] = vector<bool>(col, flag);
		}
		return contrast;
	}

	vector<Coord> CreateRandCoords(int row, int col){
		int yRange = row * 3;
		int xRange = col * 3;
		int numRandCoord = row * col / 2;

		vector<Coord> coords;
		for (int i = 0; i < numRandCoord; i++){
			int x = (rand() % xRange) - col;
			int y = (rand() % yRange) - row;
			coords.push_back({ x, y });
		}

		return coords;
	}

	void SetFlag(TwoDimensionalMark* mark,  vector<vector<bool> >* contrast,  
				const MarkSize& size, const vector<Coord>& coords, bool flag){
		for (auto c : coords){
			mark->set(c.x, c.y, flag);
			//随机位置有可能在row，col之外，要防止溢出
			if (c.x >= 0 && c.x < size.col && c.y >= 0 && c.y < size.row){
				(*contrast)[c.y][c.x] = flag;
			}
		}
	}
};

//行数多于列数的情况
INSTANTIATE_TEST_CASE_P(RowLTCol, TestWithMarkSize,	testing::Values(MarkSize(200, 100), MarkSize(50, 30)));
//行数少于列数的情况
INSTANTIATE_TEST_CASE_P(ColLTRow, TestWithMarkSize,	testing::Values(MarkSize(10, 100), MarkSize(3, 10)));
//行数等于列数的情况
INSTANTIATE_TEST_CASE_P(RowEQCol, TestWithMarkSize,	testing::Values(MarkSize(100, 100), MarkSize(3, 3)));


TEST_P(TestWithMarkSize,testWithTrueClean){
	MarkSize size = GetParam();

	//初始化TwoDimensionalMark和用来做对比的contrast,将他们全部用true填充
	TwoDimensionalMark mark(size.row, size.col);
	mark.clean(true);
	vector<vector<bool> > contrast = CreateContrast(size.row, size.col, true);

	//验证mark和contrast里面都是true
	for (int i = 0; i < size.row; i++){
		for (int j = 0; j < size.col; j++){
			ASSERT_TRUE(mark.check(j, i));
			ASSERT_TRUE(contrast[i][j]);
		}
	}

	//随机将一些坐标位置,这个函数生成的随机位置有可能在row，col之外
	vector<Coord> coords = CreateRandCoords(size.row, size.col);

	//将mark和contrast的之前随机生成的位置设为false
	SetFlag(&mark, &contrast, size, coords, false);

	//检测随机生成的位置上的值有没有设置正确
	//mark若位置超出范围则返回false,而contrast则需要防止溢出
	for (auto c : coords){
		ASSERT_FALSE(mark.check(c.x,c.y));
		if (c.x >= 0 && c.x < size.col && c.y >= 0 && c.y < size.row){
			ASSERT_FALSE(contrast[c.y][c.x]);
		}
	}

	//对比contrast和mark，两者应该相等
	for (int y = 0; y < size.row; y++)
		for (int x = 0; x < size.col; x++){
			ASSERT_EQ(contrast[y][x], mark.check(x, y));
		}
}

TEST_P(TestWithMarkSize, testWithFalseClean){
	MarkSize size = GetParam();

	//初始化TwoDimensionalMark和用来做对比的contrast,将他们全部用false填充
	TwoDimensionalMark mark(size.row, size.col);
	mark.clean(false);
	vector<vector<bool> > contrast = CreateContrast(size.row, size.col, false);

	//验证mark和contrast里面都是false
	for (int i = 0; i < size.row; i++){
		for (int j = 0; j < size.col; j++){
			ASSERT_FALSE(mark.check(j, i));
			ASSERT_FALSE(contrast[i][j]);
		}
	}

	//随机将一些坐标位置,这个函数生成的随机位置有可能在row，col之外
	vector<Coord> coords = CreateRandCoords(size.row, size.col);

	//将mark和contrast的之前随机生成的位置设为true
	SetFlag(&mark, &contrast, size, coords, true);

	//检测随机生成的位置上的值有没有设置正确
	//mark若位置超出范围则返回false,而contrast则需要防止溢出
	for (auto c : coords){
		if (c.x >= 0 && c.x < size.col && c.y >= 0 && c.y < size.row){
			ASSERT_TRUE(contrast[c.y][c.x]);
			ASSERT_TRUE(mark.check(c.x, c.y));
		}
		else{
			ASSERT_FALSE(mark.check(c.x, c.y));
		}
	}
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	int result =  RUN_ALL_TESTS();
	getchar();
	return result;
}