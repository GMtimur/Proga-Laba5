#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

const int SUBJECT_COUNT = 5;
const int NAME_LENGTH = 60;

struct Student {
    char* name;    
    char* group;
    int marks[SUBJECT_COUNT];
    float average;        

    void computeAverage(bool &anyStudentAbove4) {
        float sum = 0;
        for (int i = 0; i < SUBJECT_COUNT; ++i) {
            sum += marks[i];
        }
        average = sum / SUBJECT_COUNT;

        if (average > 4.0) {
            anyStudentAbove4 = true;
        }
    }
};

struct GroupStats {
    char* group;
    int studentCount;
    int failCount;

    GroupStats() : studentCount(0), failCount(0), group(nullptr) {}
};

void printStudentTable(Student* students, int N) {
    cout << "| " << setw(6) << "Группа" << " | " << setw(30) << "ФИО" << " | ";
    for (int i = 0; i < SUBJECT_COUNT; ++i) {
        cout << setw(3) << i + 1 << " ";
    }
    cout << "| " << setw(8) << "Средний балл" << " |" << endl;
    cout << "|--------|-----------------------------|------------------------------------|" << endl;

    int maxlen = 0;
    for (int i = 0; i < N; ++i) {
        maxlen = max(maxlen, (int)strlen(students[i].name));
    }

    for (int i = 0; i < N; ++i) {
        int padding = maxlen - strlen(students[i].name) - 1;
        cout << "| " << setw(6) << students[i].group << " | " 
             << setw(maxlen) << left << students[i].name << " | ";
        for (int j = 0; j < SUBJECT_COUNT; ++j) {
            cout << setw(3) << students[i].marks[j] << " ";
        }
        cout << "| " << setw(8) << fixed << setprecision(1) << students[i].average << setw(6) << " |" << endl;
    }

    cout << "|--------|-----------------------------|------------------------------------|" << endl;
}

void printGroupStatsTable(GroupStats* groupStats, int groupCount) {
    cout << "| " << setw(6) << "Группа" << " | " << setw(15) << "Студентов" << setw(9) << " | "
         << setw(25) << "Двоечников" << " |" << endl;
    cout << "|--------|-----------------|-----------------|" << endl;

    for (int i = 0; i < groupCount; ++i) {
        cout << "| " << setw(6) << groupStats[i].group << " | "
             << setw(15) << groupStats[i].studentCount << " | "
             << setw(15) << groupStats[i].failCount << " |" << endl;
    }

    cout << "|--------|-----------------|-----------------|" << endl;
}

void sortByGroup(Student* students, int N) {
    for (int i = 1; i < N; ++i) {
        Student key = students[i];
        int j = i - 1;
        while (j >= 0 && strcmp(students[j].group, key.group) > 0) {
            students[j + 1] = students[j];
            --j;
        }
        students[j + 1] = key;
    }
}

void sortByAverageDesc(Student* students, int N) {
    for (int i = 1; i < N; ++i) {
        Student key = students[i];
        int j = i - 1;
        while (j >= 0 && students[j].average < key.average) {
            students[j + 1] = students[j];
            --j;
        }
        students[j + 1] = key;
    }
}

void printStudentInfo(Student* students, int N) {
    for (int i = 0; i < N; ++i) {
        cout << students[i].group << " - " << students[i].name << ": ";
        for (int j = 0; j < SUBJECT_COUNT; ++j) {
            cout << students[i].marks[j];
            if (j != SUBJECT_COUNT - 1) cout << ", ";
        }
        cout << endl;
    }
}

void computeGroupStats(Student* students, int N, GroupStats*& groupStats, int& groupCount, int& groupStatsCapacity) {
    for (int i = 0; i < N; ++i) {
        bool found = false;
        for (int j = 0; j < groupCount; ++j) {
            if (strcmp(groupStats[j].group, students[i].group) == 0) {
                found = true;
                groupStats[j].studentCount++;
                for (int k = 0; k < SUBJECT_COUNT; ++k) {
                    if (students[i].marks[k] == 2) {
                        groupStats[j].failCount++;
                        break;
                    }
                }
                break;
            }
        }

        if (!found) {
            if (groupCount == groupStatsCapacity) {
                groupStatsCapacity *= 2;
                GroupStats* newGroupStats = new GroupStats[groupStatsCapacity];
                for (int k = 0; k < groupCount; ++k) {
                    newGroupStats[k] = groupStats[k];
                }
                delete[] groupStats;
                groupStats = newGroupStats;
            }

            groupStats[groupCount].group = students[i].group;
            groupStats[groupCount].studentCount = 1;
            groupStats[groupCount].failCount = 0;
            for (int k = 0; k < SUBJECT_COUNT; ++k) {
                if (students[i].marks[k] == 2) {
                    groupStats[groupCount].failCount = 1;
                    break;
                }
            }
            groupCount++;
        }
    }
}

void sortGroupStatsByFailCountDesc(GroupStats* groupStats, int groupCount) {
    for (int i = 1; i < groupCount; ++i) {
        GroupStats key = groupStats[i];
        int j = i - 1;
        while (j >= 0 && groupStats[j].failCount < key.failCount) {
            groupStats[j + 1] = groupStats[j];
            --j;
        }
        groupStats[j + 1] = key;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RU");
    bool isHuman = (argc <= 1 || strcmp(argv[1], "false") != 0);

    int N;
    cin >> N;

    Student* students = new Student[N];
    int groupStatsCapacity = 10;
    GroupStats* groupStats = new GroupStats[groupStatsCapacity];
    int groupCount = 0;
    bool anyStudentAbove4 = false;
    for (int i = 0; i < N; ++i) {
        students[i].name = new char[NAME_LENGTH];
        students[i].group = new char[NAME_LENGTH];
        cin.ignore();
        cin.getline(students[i].name, NAME_LENGTH);
        cin.getline(students[i].group, NAME_LENGTH);
        for (int j = 0; j < SUBJECT_COUNT; ++j) {
            cin >> students[i].marks[j];
        }
        students[i].computeAverage(anyStudentAbove4);
    }

    sortByGroup(students, N);
    if (!isHuman) printStudentInfo(students, N);

    computeGroupStats(students, N, groupStats, groupCount, groupStatsCapacity);
    sortGroupStatsByFailCountDesc(groupStats, groupCount);

    sortByAverageDesc(students, N);

    if (isHuman) {
        printStudentTable(students, N);
    } else {
        if (anyStudentAbove4) {
            for (int i = 0; i < N; ++i) {
                if (students[i].average > 4.0) {
                    cout << students[i].group << ", " << students[i].name << " - " << students[i].average << endl;
                }
            }
        } else {
            cout << "NO" << endl;
        }
    }

    if (isHuman) {
        printGroupStatsTable(groupStats, groupCount);
    }
    
    for (int i = 0; i < N; ++i) {
        delete[] students[i].name;
        delete[] students[i].group;
    }
    delete[] students;
    delete[] groupStats;

    return 0;
}
