#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace std;

const int SUBJECT_COUNT = 5;
const int NAME_LENGTH = 60;

int myStrcmp(const char* a, const char* b) {
    int result = 0;
    while (*a == *b && *a != '\0') {
        a++;
        b++;
    }
    if (*a == '\0' && *b == '\0') {
        return 0;
    }
    if (*a > *b) {
        return 1;
    } 
    else {
        return -1;
    }
}


char* myStrcpy(char* b, const char* str2) {
    int i;
    for (i = 0; str2[i]; i++) {
        b[i] = str2[i];
    }
    b[i] = '\0'; // Null-terminate the string
    return b;
}

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

void tab(double a, string h){
	for (int i = 0; i < a; i++)
		cout << h;
}

void header(struct Student s[], int a1, int i){
	string A = " " + string(s[i].name) + " ";
	cout << "|  " << s[i].group << "  ||";
	// tab((a1 - A.length()) / 2, "_");
	cout << A;
	tab((a1 - A.length()) / 2, " ");
	if ((a1 - A.length()) % 2 == 1)
		cout << " ";
}

void table(struct Student s[], int a1, int a2, int N, int h){
	cout << "___________";
	tab(a1, "_");
	if (h == 1)
		cout << "______________________" << endl;
	if (h == 2)
		cout << "____________________" << endl;
	cout << "| Группа ||";
	tab(a2, " ");
	cout << "ФИО";
	tab(a2, " ");
	if ((a1 - 3) % 2 == 1)
		cout << " ";
	if (h == 1)
	    cout << "||       Оценки      |" << endl;
	if (h == 2)
		cout << "||  Средний балл   |" << endl;
	cout << "___________";
	tab(a1, "_");
	if (h == 1)
	{
		cout << "______________________" << endl;
		for (int i = 0; i < N; i++)
		{
			header(s, a1, i);
            tab(a2, " ");
			cout << "|";
			for (int j = 0; j < 5; j++)
				cout << "| " << s[i].marks[j] << " ";
			cout << "|" << endl;
		}
	}
	if (h == 2)
	{
		cout << "____________________" << endl;
		for (int x = 25; x > 20; x--)
			for (int i = 0; i < N; i++)
				if (x == s[i].average * 5)
				{
					header(s, a1, i);
                    tab(a2, " ");
					if (x / 5.0 == 5)
						cout << setprecision(2) << "||        " << x / 5.0 << "        |" << endl;
					else
						cout << setprecision(2) << "||       " << x / 5.0 << "       |" << endl;
				}
	}
	cout << "___________";
	tab(a1, "_");
	if (h == 1)
		cout << "______________________" << endl;
	if (h == 2)
		cout << "____________________" << endl;
	cout << endl;
}

void tableGroup(struct GroupStats M[], int K){
	cout << "Таблица студентов без двоек:" << endl;
	cout << "____________________________________________________________" << endl;
	cout << "| Группа || Кол-во студентов || Кол-во студентов без двоек |" << endl;
	cout << "____________________________________________________________" << endl;
	for (int i = 0; i < K; i++)
	{
		if (M[i].group != nullptr) {
            cout << "|  " << M[i].group << "  ||";
        } else {
            cout << "|  [UNKNOWN GROUP]  ||";
        }
		tab((18 - (to_string(M[i].studentCount)).size()) / 2, " ");
		cout << M[i].studentCount;
		tab((18 - (to_string(M[i].studentCount)).size()) / 2, " ");
		if ((to_string(M[i].studentCount)).size() % 2 == 1)
			cout << " ";
		cout << "||";
		tab((28 - (to_string(M[i].failCount)).size()) / 2, " ");
		cout << M[i].failCount;
		tab((28 - (to_string(M[i].failCount)).size()) / 2, " ");
		if ((to_string(M[i].studentCount)).size() % 2 == 1)
			cout << " ";
		cout << "|" << endl;
	}
	cout << "____________________________________________________________" << endl;
}

void sortByGroup(Student* students, int N) {
    for (int i = 1; i < N; ++i) {
        Student key = students[i];
        int j = i - 1;
        while (j >= 0 && myStrcmp(students[j].group, key.group) > 0) {
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

void printGroupStats(GroupStats* groupStats, int groupCount) {
    for (int i = 0; i < groupCount; ++i) {
        cout << groupStats[i].group << " - "
             << groupStats[i].studentCount << " - "
             << groupStats[i].failCount << endl;
    }
}



void computeGroupStats(Student* students, int N, GroupStats*& groupStats, int& groupCount, int& groupStatsCapacity) {
    for (int i = 0; i < N; ++i) {
        bool found = false;
        for (int j = 0; j < groupCount; ++j) {
            if (myStrcmp(groupStats[j].group, students[i].group) == 0) {
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

            groupStats[groupCount].group = new char[strlen(students[i].group) + 1];
            myStrcpy(groupStats[groupCount].group, students[i].group);
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

int ms(struct Student s[], int N, int maxsize)
{
	for (int i = 0; i < N; i++)
	{
		string A = " " + string(s[i].name) + " ";
		if (A.length() > maxsize)
			maxsize = A.length();
	}
	return maxsize;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RU");
    bool isHuman = (argc <= 1 || myStrcmp(argv[1], "false") != 0);

    int N;
    cin >> N;

    // Динамическое выделение памяти для студентов
    Student* students = new Student[N];
    // Начальный размер массива для статистики групп
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
    int maxsize = 0;
	maxsize = ms(students, N, maxsize);

    if (isHuman) {
		table(students, maxsize, (maxsize - 3) / 2, N, 1);
        int d = 0;
		for (int x = 25; x > 20; x--)
			for (int i = 0; i < N; i++)
				if (x == students[i].average * 5)
					d++;
		if (d == 0)
			cout << "Студентов со средним баллом больше 4.0 нет" << endl;
		else
		{
			cout << "Таблица успешных студентов (с баллом выше 4.0):" << endl;
			table(students, maxsize, (maxsize - 3) / 2, N, 2);
		}
        cout << endl;
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
        tableGroup(groupStats, groupStatsCapacity);
    } else {
        printGroupStats(groupStats, groupCount);
    }

    for (int i = 0; i < N; ++i) {
        delete[] students[i].name;
        delete[] students[i].group;
    }
    for (int i = 0; i < groupCount; ++i) {
        if (groupStats[i].group != nullptr) {
            delete[] groupStats[i].group;
        }
    }
    delete[] students;
    delete[] groupStats;


    return 0;
}
