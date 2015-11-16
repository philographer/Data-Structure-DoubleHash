#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>


using namespace std;

struct student{
    int id;
    int credit;
    string email;
};

class students_table {
private:
    student **students;
    int student_num;
    int max_size;
    int q;
    int probe_num = 0;
    float load_factor = 0;
public:
    students_table(int, int); // 생성자
    ~students_table();
    void insert_student(int, int, string);
    int double_hashing_insert(int, int, int);
    int double_hashing_search(int, int, int);
    int get_probe_num();
    float get_load_factor();
    int first_hashing(int);
    void student_print(int);
    void modify_email(int, string);
    void update_credit(int, int);
};

students_table::students_table(int _max_size, int _q){ // 생성자
    students = new student*[_max_size];
    for (int i = 0; i < _max_size; i++){
        students[i] = NULL;
    }
    max_size = _max_size;
    q = _q;
}
students_table::~students_table(){
    delete students;
}

float students_table::get_load_factor(){
    return load_factor;
}

int students_table::get_probe_num(){
    return probe_num;
}

void students_table::update_credit(int _id, int _credit){
    probe_num = 0;
    int probe = first_hashing(_id);
    if (students[probe] != NULL)// 1차해싱에서 값이 null이 아니면
    {
        probe_num++;
        if (students[probe]->id == _id) // 1차해싱값이 맞을때
        {
            if (students[probe]->credit + _credit > 24)
                cout << "Exceeded" << endl;
            else
            {
                students[probe]->credit = students[probe]->credit + _credit;
                cout << probe_num << " " << students[probe]->credit << endl;
            }
        }
        else// 1차해싱이 아닐때
        {
            int double_hashed_num = double_hashing_search(probe, q, _id);
            if (double_hashed_num != -1)
            {
                if (students[double_hashed_num]->credit + _credit > 24)
                    cout << "Exceeded" << endl;
                else
                {
                    students[double_hashed_num]->credit = students[double_hashed_num]->credit + _credit;
                    cout << probe_num << " " << students[double_hashed_num]->credit << endl;
                }
            }
            else
                cout << probe_num << " Not found" << endl;
        }
    }
    else// 1차 해싱값이 null => 존재 x
        cout << "1" << " Not found" << endl;
}

void students_table::modify_email(int _id, string _email){
    probe_num = 0;
    int probe = first_hashing(_id);
    if (students[probe] != NULL)// 1차해싱에서 값이 null이 아니면
    {
        probe_num++;
        if (students[probe]->id == _id) // 1차해싱값이 맞을때
        {
            students[probe]->email = _email;
            cout << probe_num << endl;;
        }
        else// 1차해싱이 아닐때
        {
            int double_hashed_num = double_hashing_search(probe, q, _id);
            if (double_hashed_num != -1)
            {
                students[double_hashed_num]->email = _email;
                cout << probe_num << endl;
            }
            else
                cout << probe_num << " Not found" << endl;
        }
    }
    else// 1차 해싱값이 null => 존재 x
        cout << "1" << " Not found" << endl;
}

void students_table::student_print(int _id){
    probe_num = 0;
    int probe = first_hashing(_id);
    if (students[probe] != NULL)// 1차해싱에서 값이 null이 아니면
    {
        probe_num++;
        if (students[probe]->id == _id)//첫 해싱에서 찾음
            cout << probe_num <<" "<< students[probe]->id <<" "<< students[probe]->credit <<" "<< students[probe]->email << endl;
        else//첫 해싱에서 못 찾고 2차해싱
        {
            int double_hashed_num = double_hashing_search(probe, q, _id);
            if (double_hashed_num != -1)
                cout << probe_num <<" "<< students[double_hashed_num]->id <<" "<< students[double_hashed_num]->credit << " " << students[double_hashed_num]->email<< endl;
            else
                cout << probe_num <<" Not found"<< endl;
        }
    }
    else// 1차 해싱값이 null => 존재 x
        cout << "1" << " Not found" << endl;
}

int students_table::first_hashing(int _id){
    return _id % max_size; // h1 = y Mod N 1차해싱
}

int students_table::double_hashing_search(int _first_hashed_num, int _q, int _id){ //탐색을 위한 2차해싱
    int double_hashed_num = 0;
    int probe = 0;
    double_hashed_num = _q - (_id % _q);//h2 = q - kmodq
    do{
        probe = (_first_hashed_num + double_hashed_num * (probe_num)) % max_size;
        probe_num++; //probe num 계산
        if (students[probe] == NULL)
        {
            probe = -1;
            break;
        }
        
    } while (students[probe]->id != _id);
    
    return probe;
}

int students_table::double_hashing_insert(int _first_hashed_num, int _q, int _id){ //삽입을 위한 2차해싱
    int double_hashed_num = 0;
    int probe = 0;
    int loopNum = 0;
    load_factor = 0;
    double_hashed_num = _q - (_id % _q);//h2 = q - kmodq
    do{
        if (loopNum > max_size)
        {
            probe = -1;
            break;
        }
        probe = (_first_hashed_num + double_hashed_num * (probe_num)) % max_size;
        probe_num++; //probe num 계산
        loopNum++;
        
    } while (students[probe] != NULL);
    
    return probe; //probe가 -1 이면 빈자리 not found
}

void students_table::insert_student(int _id, int _credit, string _email){
    probe_num = 0;
    int first_hashed_num = _id % max_size; // h1 = y Mod N 1차해싱
    probe_num++;
    
    if (students[first_hashed_num] == NULL) // first hashing에서 없으면 집어넣음
    {
        students[first_hashed_num] = new student;
        students[first_hashed_num]->id = _id;
        students[first_hashed_num]->credit = _credit;
        students[first_hashed_num]->email = _email;
    }
    
    else{ //first hashing한 값이 있으면 Double Hashing
        int probe = double_hashing_insert(first_hashed_num, q, _id);
        students[probe] = new student;
        students[probe]->id = _id;
        students[probe]->credit = _credit;
        students[probe]->email = _email;
    }
    student_num++;
    
    float tmp_loadFactor = (float)student_num / (float)max_size;//loadFactor계산
    tmp_loadFactor = (floor((tmp_loadFactor)* pow(10, 2) + 0.5) / pow(10, 2)); //소수점 2째짜리 반올림
    load_factor = tmp_loadFactor;
}




int main() {
    string fileName;                    //파일이름
    string input_line_array[10001];     //입력되는 학생 배열 최대 10,000 + 1(첫줄은 테이블 생성에 받는 인자 N, 현재수, q)
    string input_line[3];
    int read_line = 0;
    int line = 0;
    int question_count = 0;
    ifstream fileIn;                    //ifstream
    getline(cin, fileName);             //파일이름 입력받음
    fileIn.open(fileName, ios::binary);  //파일을 연다.
    if (!fileIn)
        cout << "해당하는 파일명의 파일이 존재하지 않습니다." << endl;
    else{
        while (!fileIn.eof()){
            getline(fileIn, input_line_array[line]);
            line++;
        }
        
        //첫라인으로 생성자 생성
        stringstream firstLine(input_line_array[0]);
        for (int i = 0; i<3; i++){
            firstLine >> input_line[i];
        }
        if (atoi(input_line[0].c_str()) > 15000) // 테이블 최대크기 15000, 15000 넘으면 종료
        {
            cout << "테이블 크기를 초과하였습니다" << endl;
            return -1;
        }
        students_table studentTable(atoi(input_line[0].c_str()), atoi(input_line[2].c_str())); //학생테이블 생성
        read_line = atoi(input_line[1].c_str());
        
        //둘째줄부터 읽으면서 학생테이블에 추가
        for (int i = 0; i < read_line; i++){
            stringstream ssin(input_line_array[i + 1]);
            for (int i = 0; i<3; i++){
                ssin >> input_line[i];
            }
            studentTable.insert_student(atoi(input_line[0].c_str()), atoi(input_line[1].c_str()), input_line[2]);
        }
        
        while (question_count < 10000){ // Q동작시 나가고 최대질의 10,000개
            question_count++;
            cin.clear();
            char input1;
            int input2, input3;
            string input4;
            cin >> input1;
            
            if (input1 == 'Q')
                break;
            
            switch (input1) {
                case 'A':
                    cin >> input2;
                    cin.ignore();//공백 한자 무시
                    getline(cin, input4);
                    studentTable.insert_student(input2, 0, input4);
                    cout << studentTable.get_probe_num() << " " << studentTable.get_load_factor() << endl;
                    break;
                case 'P':
                    cin >> input2;
                    studentTable.student_print(input2);
                    break;
                case 'U':
                    cin >> input2 >> input3;
                    studentTable.update_credit(input2, input3);
                    break;
                case 'M':
                    cin >> input2;
                    cin.ignore();//공백 한자 무시
                    getline(cin, input4);
                    studentTable.modify_email(input2, input4);
                    break;
                default:
                    cout << "Wrong Question" << endl;
                    break;
            }
        }
    }
    cout << "Program End" << endl;
    return 0;
}