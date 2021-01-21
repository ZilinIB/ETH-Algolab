#include <iostream>
#include <vector>
#include <limits>

using namespace std;

struct Word {
    int pos, index;

    Word(int p, int i) : pos(p), index(i) {}
};

bool operator < (const Word w1, const Word w2) {
    return w1.pos < w2.pos;
}

void testcase() {
    int n; cin >> n;
    vector<int> freq;
    for (int i = 0; i < n; i++) {
        int temp; cin >> temp;
        freq.push_back(temp);
    }

    vector<Word> words;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < freq[i]; j++) {
            int position; cin >> position;
            Word w(position, i);
            words.push_back(w);
        }
    }

    sort(words.begin(), words.end());

    vector<int> count_per_word(n, 0);
    int num_words = 0;
//    count_per_word[words[0].index]++;

    int N = words.size();
    int l = 0, r = 0;

    int len = INT_MAX;

    while(r <= N) {
        if (num_words < n) {
            if (r == N) {
                break;
            }
            int ind = words[r].index;
            r++;
            if (count_per_word[ind] == 0) {
                num_words++;
            }
            count_per_word[ind]++;
            
        } else {
            int ind = words[l].index;
            l++;
            count_per_word[ind]--;
            if (count_per_word[ind] == 0) {
                num_words--;
            }
        }

        if (num_words == n) {
            int curr_len = words[r - 1].pos - words[l].pos + 1;
            if (curr_len < len) {
                len = curr_len;
            }
        }
    }
    cout << len << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}