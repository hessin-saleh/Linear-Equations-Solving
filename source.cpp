#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cmath>

using namespace std;

string floatToStr(float v) {
    ostringstream ss;
    ss << v;
    return ss.str();
}

class Equation {
public:
    map<string, float> variables;
    float constant_value;

    Equation() { constant_value = 0.0; }

    string left_side(string form) {
        int eq = form.find("=");
        return form.substr(0, eq);
    }

    string right_side(string form) {
        int eq = form.find("=");
        return form.substr(eq + 1);
    }

    vector<string> extract_terms(string side) {
        vector<string> terms;
        string cur = "";

        for (int i = 0; i < (int)side.length(); i++) {
            if ((side[i] == '+' || side[i] == '-') && !cur.empty()) {
                terms.push_back(cur);
                cur = "";
            }
            cur += side[i];
        }

        if (!cur.empty()) terms.push_back(cur);

        return terms;
    }

    void parse_term(string term, bool is_left_side) {
        int indexcut = -1;
        for (int i = 0; i < (int)term.length(); i++) {
            if (isalpha(term[i])) { indexcut = i; break; }
        }

        if (indexcut != -1) {
            string value = term.substr(0, indexcut);
            string key   = term.substr(indexcut);
            float valueNum;

            if (value == "" || value == "+")  valueNum = 1.0;
            else if (value == "-")             valueNum = -1.0;
            else                               valueNum = atof(value.c_str());

            if (!is_left_side) valueNum *= -1;

            variables[key] += valueNum;

        } else {
            float constNum = atof(term.c_str());

            if (is_left_side) constNum *= -1;

            constant_value += constNum;
        }
    }

    void build_equation(string form) {
        string left  = left_side(form);
        string right = right_side(form);

        for (auto& t : extract_terms(left))  parse_term(t, true);
        for (auto& t : extract_terms(right)) parse_term(t, false);
    }

    void print_equation() const {
        bool first = true;

        for (auto& p : variables) {
            if (p.second == 0.0) continue;

            float coef = p.second;

            if (!first) {
                if (coef > 0) cout << "+";
            }

            cout << coef << p.first;
            first = false;
        }

        if (first) cout << "0";

        cout << "=" << constant_value << "\n";
    }

    float get_coef(const string& var) const {
        auto it = variables.find(var);
        if (it != variables.end()) return it->second;
        return 0.0;
    }

    Equation add(const Equation& other) const {
        Equation result;

        result.variables = variables;
        result.constant_value = constant_value;

        for (auto& p : other.variables)
            result.variables[p.first] += p.second;

        result.constant_value += other.constant_value;

        return result;
    }

    Equation subtract(const Equation& other) const {
        Equation result;

        result.variables = variables;
        result.constant_value = constant_value;

        for (auto& p : other.variables)
            result.variables[p.first] -= p.second;

        result.constant_value -= other.constant_value;

        return result;
    }

    Equation scale(float s) const {
        Equation result;
        for (auto& p : variables)
            result.variables[p.first] = p.second * s;
        result.constant_value = constant_value * s;
        return result;
    }

    Equation substitute(const string& var, const Equation& expr) const {
        float c1 = get_coef(var);
        if (c1 == 0.0) return *this;

        float c2 = expr.get_coef(var);
        if (c2 == 0.0) return *this;

        Equation scaled = expr.scale(c1 / c2);

        return this->subtract(scaled);
    }
};

set<string> get_all_vars(const vector<Equation>& eqs) {
    set<string> vars;

    for (auto& e : eqs)
        for (auto& p : e.variables)
            vars.insert(p.first);

    return vars;
}

float determinant(vector<vector<float>>& mat, int n) {
    if (n == 1) return mat[0][0];

    if (n == 2) return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];

    float det = 0;

    for (int col = 0; col < n; col++) {
        vector<vector<float>> sub(n-1, vector<float>(n-1));
        for (int r = 1; r < n; r++) {
            int sc = 0;
            for (int c = 0; c < n; c++) {
                if (c == col) continue;
                sub[r-1][sc++] = mat[r][c];
            }
        }

        float sign = (col % 2 == 0) ? 1.0f : -1.0f;

        det += sign * mat[0][col] * determinant(sub, n-1);
    }

    return det;
}

int main() {
    int n;
    vector<Equation> equations;

    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);

        Equation eq;
        eq.build_equation(line);
        equations.push_back(eq);
    }

    string command;
    while (getline(cin, command)) {

        if (command == "quit") break;

        istringstream iss(command);
        string op;
        iss >> op;

        if (op == "num_vars") {
            set<string> vars = get_all_vars(equations);
            cout << vars.size() << "\n";
        }
        else if (op == "equation") {
            int idx;
            iss >> idx;
            equations[idx - 1].print_equation();
        }
        else if (op == "column") {
            string var;
            iss >> var;
            for (auto& eq : equations)
                cout << eq.get_coef(var) << "\n";
        }
        else if (op == "add") {
            int i, j;
            iss >> i >> j;
            Equation result = equations[i-1].add(equations[j-1]);
            result.print_equation();
        }
        else if (op == "subtract") {
            int i, j;
            iss >> i >> j;
            Equation result = equations[i-1].subtract(equations[j-1]);
            result.print_equation();
        }
        else if (op == "substitute") {
            string var;
            int i, j;
            iss >> var >> i >> j;
            Equation result = equations[i-1].substitute(var, equations[j-1]);
            result.print_equation();
        }
        else if (op == "D") {
            string token;
            iss >> token;

            set<string> varsSet = get_all_vars(equations);
            vector<string> vars(varsSet.begin(), varsSet.end());
            int sz = vars.size();

            if (token.empty()) {
                for (int r = 0; r < n; r++) {
                    for (int c = 0; c < sz; c++) {
                        if (c > 0) cout << " ";
                        cout << equations[r].get_coef(vars[c]);
                    }
                    cout << "\n";
                }
            } else {
                int replaceCol = -1;
                for (int c = 0; c < sz; c++)
                    if (vars[c] == token) { replaceCol = c; break; }

                for (int r = 0; r < n; r++) {
                    for (int c = 0; c < sz; c++) {
                        if (c > 0) cout << " ";
                        if (c == replaceCol)
                            cout << equations[r].constant_value;
                        else
                            cout << equations[r].get_coef(vars[c]);
                    }
                    cout << "\n";
                }
            }
        }
        else if (op == "D_value") {
            set<string> varsSet = get_all_vars(equations);
            vector<string> vars(varsSet.begin(), varsSet.end());
            int sz = vars.size();

            vector<vector<float>> mat(n, vector<float>(sz));
            for (int r = 0; r < n; r++)
                for (int c = 0; c < sz; c++)
                    mat[r][c] = equations[r].get_coef(vars[c]);

            cout << determinant(mat, sz) << "\n";
        }
        else if (op == "solve") {
            set<string> varsSet = get_all_vars(equations);
            vector<string> vars(varsSet.begin(), varsSet.end());
            int sz = vars.size();

            vector<vector<float>> mat(n, vector<float>(sz));
            vector<float> rhs(n);
            for (int r = 0; r < n; r++) {
                for (int c = 0; c < sz; c++)
                    mat[r][c] = equations[r].get_coef(vars[c]);
                rhs[r] = equations[r].constant_value;
            }

            float D = determinant(mat, sz);

            if (fabs(D) < 1e-9) {
                cout << "No Solution\n";
            } else {
                for (int c = 0; c < sz; c++) {
                    vector<vector<float>> modified = mat;

                    for (int r = 0; r < n; r++)
                        modified[r][c] = rhs[r];

                    float Di = determinant(modified, sz);
                    cout << vars[c] << "=" << Di / D << "\n";
                }
            }
        }
    }

    return 0;
}
