#include<stdio.h>
#include<vector>

#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //文字ストリーム
#include<tuple>

using namespace std;
int record_count = 0;
FILE *fp_record, fp_read_correct;
char record_directory[128];
char read_correct_directory[128];
int count_score = 0;
std::vector<std::vector<int>>correct_xy_point;

std::tuple<std::vector<std::vector<int>>> read_correct_xy(int data_num, char correct_scv[]) {

	//配列の定義と初期化
	std::vector<std::vector<int>>correct_xy_point;
	correct_xy_point.resize(3);
	for (int i = 0; i < 3; ++i) {
		correct_xy_point[i].resize(data_num + 1);
	}
	int i = 0;
	int j = 0;
	//char *Filename_c_s = "..\\bmp\\simulation18-0115\\correct.csv";
	sprintf(read_correct_directory, "%s", correct_scv);
	//sprintf(read_correct_directory, "%s", Filename_c_s);

	ifstream read_correct(read_correct_directory);

	string str_read_correct;

	while (getline(read_correct, str_read_correct)) {
		i = 0;
		string token_read_correct;
		istringstream stream_read_correct(str_read_correct);

		while (getline(stream_read_correct, token_read_correct, ',')) {	//一行読み取る．
			if (i > 0 && j > 0) {
				int tmp_read_correct = stof(token_read_correct);			//文字を数字に変換
				correct_xy_point[i][j] = tmp_read_correct;

			}

			++i;
		}
		++j;
	}
	for (j = 1; j<data_num + 1; ++j) {

		printf("correct_x=%d,correct_y=%d\n", correct_xy_point[1][j], correct_xy_point[2][j]);

	}


	return std::forward_as_tuple(correct_xy_point);

}


int score(int correct_x, int correct_y, int image_xt, int image_yt, std::vector<int> max_x, std::vector<int> max_y, int count_tied_V_vote, int V_vote_max, int data_num, char correct_scv[], int high_score_range_x, int high_score_range_y) {
	//得点を計算します

	//int high_score_range_x = 5;		//正解の座標の±high_score_rangeならscore 1
	//int high_score_range_y = 5;
	double middle_score_range_x_percent = 0.5;	//正解の座標からテンプレート画像サイズ×middle_score_range
	double middle_score_range_y_percent = 0.5;
	int middle_score_range_x = 0;
	int middle_score_range_y = 0;
	int correct_score = 0;
	int point_flag = 0;
	int multi_vote_flag = 0;
	int i, j;

	++count_score;

	middle_score_range_x = image_xt*middle_score_range_x_percent;
	middle_score_range_y = image_yt*middle_score_range_y_percent;

	printf("middle_score_range_x-%d,middle_score_range_y=%d\n", middle_score_range_x, middle_score_range_y);


	//配列の定義と初期化
	//	std::vector<std::vector<int>>correct_xy_point;
	if (count_score == 1) {
		correct_xy_point.resize(3);
		for (int i = 0; i < 3; ++i) {
			correct_xy_point[i].resize(data_num + 1);
		}

		std::tie(correct_xy_point) = read_correct_xy(data_num, correct_scv);
	}


	correct_x = correct_xy_point[1][count_score];
	correct_y = correct_xy_point[2][count_score];
	printf("score:correct_x=%d,correct_y=%d\n", correct_x, correct_y);

	if (count_tied_V_vote > 1)multi_vote_flag = 1;

	//完璧に同じ座標の時
	for (int k = 0; k < count_tied_V_vote; ++k) {
		if (max_x[k] == correct_x && max_y[k] == correct_y) {
			correct_score = 1;
			point_flag = 1;
		}
	}

	//座標が±high_score_rangeの時
	if (point_flag == 0) {
		for (int k = 0; k < count_tied_V_vote; ++k) {
			for (int i = correct_y - high_score_range_y; i <= correct_y + high_score_range_y; ++i) {
				for (int j = correct_x - high_score_range_x; j <= correct_x + high_score_range_x; ++j) {

					if (max_x[k] == j && max_y[k] == i) {
						correct_score = 2;
						point_flag = 1;
						break;
					}
				}
				if (point_flag == 1)break;
			}
			if (point_flag == 1)break;
		}
	}

	//座標がテンプレート画像サイズ×middle_score_rangeの時
	if (point_flag == 0) {
		for (int k = 0; k < count_tied_V_vote; ++k) {
			for (int i = correct_y - middle_score_range_y; i <= correct_y + middle_score_range_y; ++i) {
				for (int j = correct_x - middle_score_range_x; j <= correct_x + middle_score_range_x; ++j) {

					if (max_x[k] == j && max_y[k] == i) {
						correct_score = 3;
						point_flag = 1;
						break;
					}
				}
				if (point_flag == 1)break;
			}
			if (point_flag == 1)break;
		}
	}

	//どれにも当てはまらない時
	if (point_flag == 0) {
		correct_score = 4;
	}

	if (count_score == data_num)count_score = 0;

	return correct_score;
}



int score_record(char date_directory[], int data_num, char Inputimage[], int correct_score, int count_tied_V_vote, int V_vote_max, std::vector<int> max_x, std::vector<int> max_y) {


	++record_count;

	if (record_count == 1) {


		//出力結果のファイル名の指定
		char *Filename_s = "score_record.csv";

		sprintf(record_directory, "%s%s", date_directory, Filename_s);


		if ((fp_record = fopen(record_directory, "w")) == NULL) {
			printf("ファイル：%sが開けません\n", record_directory);
			exit(1);
		}
		fprintf(fp_record, "判定,count_tied_V_vote,V_vote_max,x,y,Inputimage\n");
	}

	if (count_tied_V_vote > 1) {
		max_x[0] = 999999;
		max_y[0] = 999999;
	}

	switch (correct_score) {
	case 1:
		fprintf(fp_record, "◎,%d,%d,%d,%d,%s\n", count_tied_V_vote, V_vote_max, max_x[0], max_y[0], Inputimage);
		break;
	case 2:
		fprintf(fp_record, "○,%d,%d,%d,%d,%s\n", count_tied_V_vote, V_vote_max, max_x[0], max_y[0], Inputimage);
		break;
	case 3:
		fprintf(fp_record, "△,%d,%d,%d,%d,%s\n", count_tied_V_vote, V_vote_max, max_x[0], max_y[0], Inputimage);
		break;
	case 4:
		fprintf(fp_record, "×,%d,%d,%d,%d,%s\n", count_tied_V_vote, V_vote_max, max_x[0], max_y[0], Inputimage);
		break;
	default:
		printf("correct_scoreがおかしい\ncorrect_score=%d\n", correct_score);
		exit(1);
		break;
	}


	if (record_count == data_num) {
		fclose(fp_record);
		record_count = 0;
	}
	return 0;
}