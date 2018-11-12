#pragma once
#include <vector>
#include <array>

class Vector
{
public:
	Vector() : array_data_({ 0.0, 0.0, 0.0 })
	{
	}
	Vector(std::array<double, 3> temp_data) : array_data_(temp_data)
	{
	}
	Vector(const double &x, const double &y, const double &z)
	{
		this->array_data_ = { x, y, z };
	}
	Vector(const Vector &src)
	{
		this->array_data_ = { src[0], src[1], src[2] };
	}
	Vector(const std::initializer_list<double> &src)
	{
		size_t idx = 0;
		auto it = src.begin();
		for (; it != src.end(); ++it)
		{
			this->array_data_[idx] = *it;
			++idx;
		}
		for (; idx < this->array_data_.size(); ++idx)
		{
			this->array_data_[idx] = *src.begin();
		}
	}
	~Vector() {}
	//Vector(Location temp_data);
	inline double& operator[](size_t idx)
	{
		return this->array_data_[idx];
	}
	const inline double& operator[](size_t idx) const
	{
		return this->array_data_[idx];
	}
	const inline Vector operator=(const Vector &src)
	{
		this->array_data_ = { src[0], src[1], src[2] };
		return *this;
	}
	inline Vector operator-=(const Vector &src)
	{
		this->array_data_ = { this->array_data_[0] - src[0], this->array_data_[1] - src[1],
			this->array_data_[2] - src[2] };
		return *this;
	}
	inline Vector operator+=(const Vector &src)
	{
		this->array_data_ = { this->array_data_[0] + src[0], this->array_data_[1] + src[1],
			this->array_data_[2] + src[2] };
		return *this;
	}
	inline Vector operator+(const Vector &src)
	{
		return (Vector(this->array_data_) += src);
	}
	inline Vector operator-(const Vector &src)
	{
		return (Vector(this->array_data_) -= src);
	}
	inline bool operator==(const Vector &src)
	{
		return (this->array_data_[0] == src[0] &&
			this->array_data_[1] == src[1] &&
			this->array_data_[2] == src[2]);
	}
	inline bool operator==(const Vector &src) const
	{
		return (this->array_data_[0] == src[0] &&
			this->array_data_[1] == src[1] &&
			this->array_data_[2] == src[2]);
	}
	inline double operator*(const Vector &v_des)
	{
		double res = this->array_data_[0] * v_des[0] +
			this->array_data_[1] * v_des[1] + this->array_data_[2] * v_des[2];
		return res;
	}
	inline Vector operator*=(const double &radio)
	{
		this->array_data_[0] = this->array_data_[0] * radio;
		this->array_data_[1] = this->array_data_[1] * radio;
		this->array_data_[2] = this->array_data_[2] * radio;
		return *this;
	}
	inline Vector operator*(const double &radio)
	{
		return (Vector(this->array_data_) *= radio);
	}
private:
	std::array<double, 3> array_data_;
};


class Location
{
public:
	Location() : array_data_({ 0.0, 0.0, 0.0 })
	{
	}
	Location(std::array<double, 3> temp_data) : array_data_(temp_data)
	{
	}
	Location(const Location &src)
	{
		this->array_data_ = { src[0], src[1], src[2] };
	}
	Location(const std::initializer_list<double> &src)
	{
		size_t idx = 0;
		auto it = src.begin();
		for (; it != src.end(); ++it)
		{
			this->array_data_[idx] = *it;
			++idx;
		}
		for (; idx < this->array_data_.size(); ++idx)
		{
			this->array_data_[idx] = *src.begin();
		}
	}
	Location(const double &x, const double &y, const double &z)
	{
		this->array_data_ = { x, y, z };
	}
	~Location() {}

	double calDist(Location location)
	{
		double dis = 0;
		dis += pow(this->array_data_[0] - location[0], 2);
		dis += pow(this->array_data_[1] - location[1], 2);
		dis += pow(this->array_data_[2] - location[2], 2);
		dis = sqrt(dis);
		return dis;
	}

	inline double& operator[](size_t idx)
	{
		return this->array_data_[idx];
	}
	const inline double& operator[](size_t idx) const
	{
		return this->array_data_[idx];
	}
	inline Location operator=(const Location &src)
	{
		this->array_data_ = { src[0], src[1], src[2] };
		return *this;
	}
	inline Location operator-=(const Location &src)
	{
		this->array_data_ = { this->array_data_[0] - src[0], this->array_data_[1] - src[1],
			this->array_data_[2] - src[2] };
		return *this;
	}

	inline Location operator-=(const Vector &src)
	{
		this->array_data_ = { this->array_data_[0] - src[0], this->array_data_[1] - src[1],
			this->array_data_[2] - src[2] };
		return *this;
	}

	inline Location operator+=(const Location &src)
	{
		this->array_data_ = { this->array_data_[0] + src[0], this->array_data_[1] + src[1],
			this->array_data_[2] + src[2] };
		return *this;
	}

	inline Location operator+=(const Vector &src)
	{
		this->array_data_ = { this->array_data_[0] + src[0], this->array_data_[1] + src[1],
			this->array_data_[2] + src[2] };
		return *this;
	}

	inline Location operator*=(const double &num)
	{
		this->array_data_ = { this->array_data_[0] * num, this->array_data_[1] * num,
			this->array_data_[2] * num };
		return *this;
	}
	inline Location operator/=(const double &num)
	{
		double temp_num = 1 / num;
		return Location(*this) *= temp_num;
	}

	inline Location operator+(const Location &src)
	{
		return (Location(this->array_data_) += src);
	}

	inline Location operator+(const Location &src) const
	{
		return (Location(this->array_data_) += src);
	}

	inline Location operator+(const Vector &src)
	{
		return (Location(this->array_data_) += src);
	}

	inline Location operator+(const Vector &src) const
	{
		return (Location(this->array_data_) += src);
	}

	inline Location operator-(const Location &src)
	{
		return (Location(this->array_data_) -= src);
	}

	inline Location operator-(const Location &src) const
	{
		return (Location(this->array_data_) -= src);
	}
	inline Location operator*(const double &num) const
	{
		return (Location(this->array_data_) *= num);
	}
	inline Location operator/(const double &num) const
	{
		return (Location(this->array_data_) /= num);
	}
	inline bool operator==(const Location &src) const
	{
		return (this->array_data_[0] == src[0] &&
			this->array_data_[1] == src[1] &&
			this->array_data_[2] == src[2]);
	}

	inline bool operator!=(const Location &src) const
	{
		return !(*this == src);
	}
	size_t size() const
	{
		return this->array_data_.size();
	}

	void fill(const double & d)
	{
		this->array_data_.fill(d);
	}
	std::array<double, 3> data()
	{
		return this->array_data_;
	}
private:
	std::array<double, 3> array_data_;
};