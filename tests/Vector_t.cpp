#include "../Vector.h"

#include <gtest/gtest.h> 
#include <gmock/gmock.h>


class Vector_f : public ::testing::Test { // инициализация контейнера
protected:
    void SetUp() override {

        for (auto i = 0; i < count; ++i) {
            vector.push_back(i);
        }
    }

    void TearDown() override {}

    Vector<int> vector;
    const int count = 10;
};


template<typename T>
class mockObjForVec { //конструктор-деструктор
    T data{};
public:
    mockObjForVec() = default;

    mockObjForVec(const mockObjForVec &other) : data(other.data) {};

    explicit mockObjForVec(T t) : data(t) {};

    ~mockObjForVec() { onDtor(); };

    mockObjForVec<T> &operator=(const mockObjForVec<T> &other) {
        data = other.data;
        return *this;
    };

    MOCK_METHOD0(onDtor, void());
};


// ТЕСТЫ
TEST(Vector, Empty) {
    Vector<int> vector;

    ASSERT_EQ(vector.size(), 0);
    ASSERT_EQ(vector.begin(), static_cast<VecIterator<int>>(nullptr));
    ASSERT_EQ(vector.end(), vector.begin());
}


TEST_F(Vector_f, PushBack) {

    int sample[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    ASSERT_EQ(vector.size(), count);
    for (auto i = 0; i < count; ++i) {
        ASSERT_EQ(vector[i], sample[i]);
    }
}


TEST_F(Vector_f, InsertFront) {
    int sample[] = {10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    vector.insert(vector.begin(), 10);

    ASSERT_EQ(vector.size(), count + 1);
    for (size_t i = 0; i < vector.size(); ++i) {
        ASSERT_EQ(vector[i], sample[i]);
    }
}


TEST_F(Vector_f, InsertBack) {
    int sample[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 20};

    vector.insert(vector.end(), 20);

    ASSERT_EQ(vector.size(), count + 1);
    for (size_t i = 0; i < vector.size(); ++i) {
        ASSERT_EQ(vector[i], sample[i]);
    }
}


TEST_F(Vector_f, InsertMid) {
    int sample[] = {0, 1, 2, 3, 4, 30, 5, 6, 7, 8, 9};

    vector.insert((vector.begin() + (vector.size() / 2)), 30);

    ASSERT_EQ(vector.size(), count + 1);
    for (size_t i = 0; i < vector.size(); ++i) {
        ASSERT_EQ(vector[i], sample[i]);
    }
}


TEST_F(Vector_f, EraseFront) {
    int sample[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    vector.erase(vector.begin());

    ASSERT_EQ(vector.size(), count - 1);
    for (size_t i = 0; i < vector.size(); ++i) {
        ASSERT_EQ(vector[i], sample[i]);
    }
}

//=7=================================================================
TEST_F(Vector_f, EraseBack) {
    int sample[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    vector.erase(vector.end() - 1);

    ASSERT_EQ(vector.size(), count - 1);
    for (size_t i = 0; i < vector.size(); ++i) {
        ASSERT_EQ(vector[i], sample[i]);
    }
}

//=8=================================================================
TEST_F(Vector_f, EraseMid) {
    int sample[] = {0, 1, 2, 3, 4, 6, 7, 8, 9};

    vector.erase((vector.begin() + (vector.size() / 2)));

    ASSERT_EQ(vector.size(), count - 1);
    for (size_t i = 0; i < vector.size(); ++i) {
        ASSERT_EQ(vector[i], sample[i]);
    }
}

//=9=================================================================
TEST_F(Vector_f, GetElementOutOfRange) {

    ASSERT_THROW(vector[count], std::out_of_range);
    ASSERT_THROW(vector[-count], std::out_of_range);
}

//=10=================================================================
TEST(Vector, CopyContainer) {
    const size_t count = 10;
    Vector<size_t> vector;
    Vector<size_t> vec2;
    int sample[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (size_t i = 0; i < count; ++i) {
        vector.push_back(i);
    }
    vec2 = vector;

    ASSERT_EQ(vector.size(), vec2.size());
    for (size_t i = 0; i < vector.size(); ++i) {
        ASSERT_EQ(vector[i], sample[i]);
        ASSERT_EQ(vec2[i], vector[i]);
    }
}

//=11=================================================================
TEST(Vector, InvokeElementDtor) {
    const size_t count = 10;
    Vector<mockObjForVec<size_t>> vector;

    for (size_t i = 0; i < count; ++i) {
        vector.push_back(mockObjForVec<size_t>(i));
    }

    for (size_t i = 0; i < count; ++i) {
        EXPECT_CALL(vector[i], onDtor());
    }
}
