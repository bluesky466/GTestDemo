#include <cstring>

class TwoDimensionalMark{
public:
	TwoDimensionalMark(int row, int col, bool flag = false):row_(row),col_(col),bit_marks_(nullptr){
		bit_marks_ = new unsigned char[computeMemorySize(row,col)];
		clean(flag);
	}

	TwoDimensionalMark(const TwoDimensionalMark& cpy):row_(0),col_(0),bit_marks_(nullptr){
		*this = cpy;
	}

	~TwoDimensionalMark() {
		if (bit_marks_) delete[] bit_marks_;
	}

	const TwoDimensionalMark& operator =(const TwoDimensionalMark& cpy){
		row_ = cpy.getRow();
		col_ = cpy.getCol();
		if (bit_marks_){
			delete bit_marks_;
		}
		bit_marks_ = new unsigned char[computeMemorySize(row_,col_)];
		memcpy(bit_marks_, cpy.bit_marks_, computeMemorySize(row_, col_));
		return *this;
	}

	void clean(bool mark){
		memset(bit_marks_, mark?0xff:0, computeMemorySize(row_,col_));
	}

	void set(int x, int y, bool mark){
		if (x < 0 || x >= getCol() || y < 0 || y >= getRow()){
			return ;
		}else if (mark){
			bit_marks_[computeByteIndex(x, y)] |= (1 << computeByteOffset(x, y));
		}else{
			bit_marks_[computeByteIndex(x, y)] &= (~(1 << computeByteOffset(x, y)));
		}
	}

	bool check(int x, int y)const{
		if (x < 0 || x >= getCol() || y < 0 || y >= getRow()){
			return false;
		}
		else{
			return bit_marks_[computeByteIndex(x, y)] & (1 << computeByteOffset(x, y));
		}
	}

	int getRow() const { return row_; }
	int getCol() const { return col_; }

private:
	int row_;
	int col_;
	unsigned char* bit_marks_;

	int computeMemorySize(int row, int col)const {
		return row*col/8 + 1;
	}

	int computeByteIndex(int x, int y)const {
		return (y * col_ + x) / 8;
	}

	unsigned char computeByteOffset(int x, int y)const {
		return (y * col_ + x) & 0x7;
	}
};