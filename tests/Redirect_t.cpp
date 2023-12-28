#include "../Redirect.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class Redirect_f : public ::testing::Test { // инициализация контейнера
protected:
    void SetUp() override {

        for (auto i = 0; i < count; ++i) {
            list.push_back(i);
        }
    }

    void TearDown() override {}

    Redirect<int> list;
    const int count = 10;
};


template<typename T>
class mockObjForFwdList { // конструктор-деструктор
    T data{};
public:
    mockObjForFwdList() = default;

    mockObjForFwdList(const mockObjForFwdList &other) : data(other.data) {};

    explicit mockObjForFwdList(T t) : data(t) {};

    ~mockObjForFwdList() { onDtor(); };

    mockObjForFwdList<T> &operator=(const mockObjForFwdList<T> &other) {
        data = other.data;
        return *this;
    };

    MOCK_METHOD0(onDtor, void());
};


// ТЕСТЫ
TEST(Redirect, Empty) {
    Redirect<int> list;

    ASSERT_EQ(list.size(), 0);
    ASSERT_EQ(list.begin(), static_cast<Redirect<int>::RedirectIterator<int>>(nullptr));
    ASSERT_EQ(list.end(), list.begin());
}


TEST_F(Redirect_f, PushBack) {

    int sample[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    ASSERT_EQ(list.size(), count);
    for (auto i = 0; i < count; ++i) {
        ASSERT_EQ(list[i], sample[i]);
    }
}


TEST_F(Redirect_f, InsertFront) {
    int sample[] = {10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    list.insert(list.begin(), 10);

    ASSERT_EQ(list.size(), count + 1);
    for (size_t i = 0; i < list.size(); ++i) {
        ASSERT_EQ(list[i], sample[i]);
    }
}


TEST_F(Redirect_f, InsertBack) {
    int sample[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 20};

    list.insert(list.end(), 20);

    ASSERT_EQ(list.size(), count + 1);
    for (size_t i = 0; i < list.size(); ++i) {
        ASSERT_EQ(list[i], sample[i]);
    }
}


TEST_F(Redirect_f, InsertMid) {
    int sample[] = {0, 1, 2, 3, 4, 30, 5, 6, 7, 8, 9};

    list.insert((list.begin() + (list.size() / 2)), 30);

    ASSERT_EQ(list.size(), count + 1);
    for (size_t i = 0; i < list.size(); ++i) {
        ASSERT_EQ(list[i], sample[i]);
    }
}


TEST_F(Redirect_f, EraseFront) {
    int sample[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    list.erase(list.begin());

    ASSERT_EQ(list.size(), count - 1);
    for (size_t i = 0; i < list.size(); ++i) {
        ASSERT_EQ(list[i], sample[i]);
    }
}



TEST_F(Redirect_f, EraseBack) {
    int sample[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    auto last = list.begin();
    for (size_t i = 0; i < list.size() - 1; ++i) {
        ++last;
    }
    list.erase(last);

    ASSERT_EQ(list.size(), count - 1);
    for (size_t i = 0; i < list.size(); ++i) {
        ASSERT_EQ(list[i], sample[i]);
    }
}


TEST_F(Redirect_f, EraseMid) {
    int sample[] = {0, 1, 2, 3, 4, 6, 7, 8, 9};

    list.erase((list.begin() + (list.size() / 2)));

    ASSERT_EQ(list.size(), count - 1);
    for (size_t i = 0; i < list.size(); ++i) {
        ASSERT_EQ(list[i], sample[i]);
    }
}

TEST_F(Redirect_f, GetElementOutOfRange) {

    ASSERT_THROW(list[count], std::out_of_range);
    ASSERT_THROW(list[-count], std::out_of_range);
}


TEST_F(Redirect_f, CopyContainer) {
    Redirect<int> list2;
    int sample[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    list2 = list;

    ASSERT_EQ(list.size(), list2.size());
    for (size_t i = 0; i < list.size(); ++i) {
        ASSERT_EQ(list[i], sample[i]);
        ASSERT_EQ(list2[i], list[i]);
    }
}


TEST(Redirect, InvokeElementDtor) {
    const size_t count = 10;
    Redirect<mockObjForFwdList<size_t>> list;

    for (size_t i = 0; i < count; ++i) {
        list.push_back(mockObjForFwdList<size_t>(i));
    }

    for (size_t i = 0; i < count; ++i) {
        EXPECT_CALL(list[i], onDtor());
    }
}
