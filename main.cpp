/*
 * ============================================================
 *   University of Haripur - GPA Management System
 *   C++ Console Application
 *   Author : Hamid (DHC-40)
 * ============================================================
 *
 *  Grading logic sourced directly from the University of Haripur
 *  official grading sheet (semester system, 4.00 scale).
 *  Per-percentage NG values are hard-coded from the detailed table.
 * ============================================================
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>

// ── Grading Table ────────────────────────────────────────────
/*
 * Returns the Numerical Grade (NG) and Letter Grade for a given
 * integer percentage score (0-100).
 * Source: UoH Detailed Numerical Grades table.
 */
struct GradeInfo {
    double ng;
    std::string letter;
};

GradeInfo getGrade(double percentage) {
    int p = static_cast<int>(percentage); // truncate; fraction rounded by UoH rule

    if (p < 50)  return {0.00, "F"};
    if (p == 50) return {1.00, "D"};
    if (p == 51) return {1.08, "D"};
    if (p == 52) return {1.17, "D"};
    if (p == 53) return {1.25, "D"};
    if (p == 54) return {1.33, "D+"};
    if (p == 55) return {1.42, "D+"};
    if (p == 56) return {1.50, "D+"};
    if (p == 57) return {1.58, "D+"};
    if (p == 58) return {1.67, "C-"};
    if (p == 59) return {1.75, "C-"};
    if (p == 60) return {1.83, "C-"};
    if (p == 61) return {1.92, "C"};
    if (p == 62) return {2.00, "C"};
    if (p == 63) return {2.08, "C"};
    if (p == 64) return {2.17, "C+"};
    if (p == 65) return {2.25, "C+"};
    if (p == 66) return {2.33, "C+"};
    if (p == 67) return {2.42, "C+"};
    if (p == 68) return {2.50, "B-"};
    if (p == 69) return {2.58, "B-"};
    if (p == 70) return {2.67, "B-"};
    if (p == 71) return {2.75, "B"};
    if (p == 72) return {2.83, "B"};
    if (p == 73) return {2.92, "B"};
    if (p == 74) return {3.00, "B"};
    if (p == 75) return {3.08, "B+"};
    if (p == 76) return {3.17, "B+"};
    if (p == 77) return {3.25, "B+"};
    if (p == 78) return {3.33, "B+"};
    if (p == 79) return {3.42, "B+"};
    if (p == 80) return {3.50, "A-"};
    if (p == 81) return {3.60, "A-"};
    if (p == 82) return {3.70, "A-"};
    if (p == 83) return {3.80, "A-"};
    if (p == 84) return {3.90, "A-"};
    // 85 and above
    return {4.00, "A"};
}

// ── Subject Data Structure ────────────────────────────────────
struct Subject {
    std::string name;
    double creditHours; // 1.0 - 4.0
    double marksObtained; // out of 100
    double percentage;
    GradeInfo grade;
    double gradePoints; // NG * creditHours
};

// ── Semester Class ────────────────────────────────────────────
class Semester {
public:
    int semesterNumber;
    std::vector<Subject> subjects;
    double sgpa;
    double totalCreditHours;
    double totalGradePoints;

    Semester(int num) : semesterNumber(num), sgpa(0.0),
                        totalCreditHours(0.0), totalGradePoints(0.0) {}

    void addSubject(const Subject& s) {
        subjects.push_back(s);
        totalCreditHours  += s.creditHours;
        totalGradePoints  += s.gradePoints;
    }

    /*
     * SGPA = Sum(NG_i * CH_i) / Sum(CH_i)
     * Fraction rounded to nearest whole number for NG lookup per UoH rule,
     * but SGPA itself is reported to 2 decimal places.
     */
    void computeSGPA() {
        if (totalCreditHours == 0.0) { sgpa = 0.0; return; }
        sgpa = totalGradePoints / totalCreditHours;
    }
};

// ── CGPA Computation ──────────────────────────────────────────
/*
 * CGPA = Sum of all grade points across all semesters
 *        / Sum of all credit hours across all semesters
 */
double computeCGPA(const std::vector<Semester>& semesters) {
    double totalGP = 0.0, totalCH = 0.0;
    for (const auto& sem : semesters) {
        totalGP += sem.totalGradePoints;
        totalCH += sem.totalCreditHours;
    }
    return (totalCH == 0.0) ? 0.0 : totalGP / totalCH;
}

// ── Input Helpers ─────────────────────────────────────────────
double getDouble(const std::string& prompt, double minVal, double maxVal) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val >= minVal && val <= maxVal) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Invalid input. Enter a value between "
                  << minVal << " and " << maxVal << ".\n";
    }
}

int getInt(const std::string& prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val >= minVal && val <= maxVal) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Invalid input. Enter an integer between "
                  << minVal << " and " << maxVal << ".\n";
    }
}

std::string getString(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

// ── Display Helpers ───────────────────────────────────────────
void printSeparator(char c = '-', int width = 72) {
    std::cout << std::string(width, c) << "\n";
}

void printHeader() {
    printSeparator('=');
    std::cout << std::setw(45) << "UNIVERSITY OF HARIPUR\n";
    std::cout << std::setw(46) << "GPA MANAGEMENT SYSTEM\n";
    printSeparator('=');
    std::cout << "\n";
}

void printSemesterReport(const Semester& sem) {
    std::cout << "\n";
    printSeparator('=');
    std::cout << "  SEMESTER " << sem.semesterNumber << " REPORT\n";
    printSeparator('=');

    // Table header
    std::cout << std::left
              << std::setw(22) << "Subject"
              << std::setw(8)  << "Marks"
              << std::setw(8)  << "  %"
              << std::setw(6)  << "  CH"
              << std::setw(7)  << "  NG"
              << std::setw(8)  << " Grade"
              << std::setw(10) << " GrdPts"
              << "\n";
    printSeparator();

    for (const auto& s : sem.subjects) {
        std::cout << std::left
                  << std::setw(22) << s.name
                  << std::right
                  << std::setw(6)  << std::fixed << std::setprecision(1) << s.marksObtained
                  << std::setw(7)  << std::fixed << std::setprecision(1) << s.percentage << "%"
                  << std::setw(6)  << std::fixed << std::setprecision(1) << s.creditHours
                  << std::setw(7)  << std::fixed << std::setprecision(2) << s.grade.ng
                  << std::setw(7)  << s.grade.letter
                  << std::setw(9)  << std::fixed << std::setprecision(2) << s.gradePoints
                  << "\n";
    }

    printSeparator();
    std::cout << std::left  << std::setw(22) << "TOTALS"
              << std::right << std::setw(6)  << " "
              << std::setw(7)  << " "
              << std::setw(6)  << std::fixed << std::setprecision(1) << sem.totalCreditHours
              << std::setw(7)  << " "
              << std::setw(7)  << " "
              << std::setw(9)  << std::fixed << std::setprecision(2) << sem.totalGradePoints
              << "\n";
    printSeparator();
    std::cout << "  SGPA (Semester " << sem.semesterNumber << ") : "
              << std::fixed << std::setprecision(2) << sem.sgpa << "\n";
    printSeparator('=');
}

void printCGPAReport(const std::vector<Semester>& semesters, double cgpa) {
    std::cout << "\n";
    printSeparator('=');
    std::cout << "  CUMULATIVE GPA SUMMARY\n";
    printSeparator('=');
    std::cout << std::left
              << std::setw(14) << "Semester"
              << std::setw(12) << "Cred.Hrs"
              << std::setw(14) << "Grade Pts"
              << std::setw(10) << "SGPA"
              << "\n";
    printSeparator();

    double totalCH = 0.0, totalGP = 0.0;
    for (const auto& sem : semesters) {
        std::cout << std::left
                  << std::setw(14) << ("Semester " + std::to_string(sem.semesterNumber))
                  << std::right
                  << std::setw(8)  << std::fixed << std::setprecision(1) << sem.totalCreditHours
                  << std::setw(12) << std::fixed << std::setprecision(2) << sem.totalGradePoints
                  << std::setw(10) << std::fixed << std::setprecision(2) << sem.sgpa
                  << "\n";
        totalCH += sem.totalCreditHours;
        totalGP += sem.totalGradePoints;
    }

    printSeparator();
    std::cout << std::left
              << std::setw(14) << "TOTAL"
              << std::right
              << std::setw(8)  << std::fixed << std::setprecision(1) << totalCH
              << std::setw(12) << std::fixed << std::setprecision(2) << totalGP
              << "\n";
    printSeparator('=');
    std::cout << "  CGPA : " << std::fixed << std::setprecision(2) << cgpa << " / 4.00\n";
    printSeparator('=');
}

// ── Main ──────────────────────────────────────────────────────
int main() {
    printHeader();

    std::string studentName = getString("  Student Name : ");
    std::string rollNo      = getString("  Roll Number  : ");
    int numSemesters = getInt("  Number of Semesters to enter (1-8) : ", 1, 8);

    std::vector<Semester> semesters;

    for (int i = 1; i <= numSemesters; i++) {
        std::cout << "\n";
        printSeparator('-');
        std::cout << "  Entering data for Semester " << i << "\n";
        printSeparator('-');

        Semester sem(i);
        int numSubjects = getInt("  Number of subjects (2-10) : ", 2, 10);

        for (int j = 1; j <= numSubjects; j++) {
            std::cout << "\n  -- Subject " << j << " --\n";
            Subject s;
            s.name         = getString("    Subject Name  : ");
            s.creditHours  = getDouble("    Credit Hours (1.0-4.0) : ", 1.0, 4.0);
            s.marksObtained= getDouble("    Marks Obtained (out of 100) : ", 0.0, 100.0);

            // Marks ARE out of 100, so percentage == marks
            s.percentage   = s.marksObtained;

            // UoH rule: fraction of percentage is rounded to whole number for NG lookup
            s.grade        = getGrade(s.percentage);
            s.gradePoints  = s.grade.ng * s.creditHours;

            sem.addSubject(s);
        }

        sem.computeSGPA();
        semesters.push_back(sem);
        printSemesterReport(sem);
    }

    double cgpa = computeCGPA(semesters);
    printCGPAReport(semesters, cgpa);

    std::cout << "\n  Student : " << studentName
              << "  |  Roll No : " << rollNo
              << "  |  CGPA : " << std::fixed << std::setprecision(2) << cgpa << "\n\n";

    return 0;
}
