/* Hiteesh Nukalapati, 116421157, hnuk*/
#include<stdio.h>
#include<math.h>
#define MAX_ASSIGNMENTS 50

int assignment_info(int num_of_assignments, int assignment_scores[], 
                    int weight[], int late[]);

void statdisplay(char c, int num_of_assignments, int penalty_per_day, 
                 int num_to_drop, int sum_of_weights, int assignment_scores[], 
                 int weight[], int late[]);

int main() {
    char stats_disp = 'N';
    int num_assignments = 0, num_to_drop = 0, penalty_per_day = 0; 
    int assignment_scores[MAX_ASSIGNMENTS] = {0}, late[MAX_ASSIGNMENTS] = {0};
    int weight[MAX_ASSIGNMENTS] = {0}, sum_of_weights = 0;

    scanf("%d %d %c %d",&penalty_per_day, &num_to_drop, &stats_disp, 
          &num_assignments); 
    
    sum_of_weights = assignment_info(num_assignments,  assignment_scores, 
          weight, late);
    
    if (sum_of_weights != 100) {
        printf("ERROR: Invalid values provided\n");
        return 0;
    }
    
    statdisplay(stats_disp,num_assignments, penalty_per_day, num_to_drop, 
                sum_of_weights, assignment_scores, weight, late);

    return 0;
}

/*Method that takes input from the user/keyboard to pupulate the feild and 
  variables from the main method. This method return the sum of weigths,
  used to check for valid input*/
int assignment_info(int num_of_assignments, int assignment_scores[], 
                    int weight[], int late[]) {
    int index = 0;
    int assingment_number = 0, days_late = 0;
    int student_score = 0;
    int percent_grade = 0;
    int sum_of_weights = 0;

    for (index = 0; index < num_of_assignments; index++) {
        scanf("%d, %d, %d, %d",&assingment_number, &student_score, 
              &percent_grade, &days_late);
        assignment_scores[assingment_number - 1] = student_score;
        weight[assingment_number - 1] = percent_grade;
        late[assingment_number - 1] = days_late;
    }

    for (index = 0; index < num_of_assignments; index++) {
        sum_of_weights += weight[index];
    }

    return sum_of_weights;
}

/*Method that calculates all the statistics including Mean, Standard Deviation  
  and Numeric Score. This Method intially calculates the mean and 
  Standard Deviation first, followed by calculating the numeric score
  after deducting any late penalties, if any*/
void statdisplay(char c, int num_of_assignments, int penalty_per_day, 
                 int num_to_drop, int sum_of_weights, int assignment_scores[], 
                 int weight[], int late[]) {

    double numeric_score = 0, mean = 0; 
    double standard_deviation = 0, sum_of_differences = 0;
    int index = 0, temp[MAX_ASSIGNMENTS] = {0};
    
    for (index = 0 ; index < num_of_assignments; index++) {
        if (late[index] > 0) {
            temp[index] = assignment_scores[index] - 
                          (penalty_per_day*late[index]);
            if (temp[index] < 0) {
                temp[index] = 0;
            }
        } else {
            temp[index] = assignment_scores[index];
        }
        numeric_score += ((double)temp[index]) * ((double)weight[index] / 100);
        mean += temp[index];
    }

    mean = mean/num_of_assignments;

    for (index = 0 ; index < num_of_assignments; index++) {
        sum_of_differences += pow((((double)temp[index]) - mean), 2);
    }    

    /*variance = sum_of_differences/num_of_assignments and square root of
      variance is standard deviation*/
    standard_deviation = sqrt(sum_of_differences/(double)num_of_assignments);
    
    if (num_to_drop > 0) {
        /*temporary variables used to find lowest value to be dropped*/
        int temp_index = 0, temp_num_to_drop = 0;
        double temp_val = 100;/*max passible value for any assignment*/
        double value[MAX_ASSIGNMENTS] = {0};
        numeric_score = 0;
        temp_num_to_drop = num_to_drop;

        for (index = 0 ; index < num_of_assignments; index++) {
            value[index] = (double)assignment_scores[index] * 
                           ( (double)(weight[index]/ 100) );
        } 

        while (temp_num_to_drop > 0) {
            for (index = 0; index < num_of_assignments; index++) {
                if (temp[index] != 0 && value[index] <= temp_val) {
                    temp_val = value[index];
                    temp_index = index;
                }
            }
            temp[temp_index] = 0;
            sum_of_weights -= weight[temp_index];
            temp_val = 100;
            temp_num_to_drop--;    
        }

        for (index = 0 ; index < num_of_assignments; index++) {
            numeric_score += ( (double)temp[index] *
                              ((double)weight[index] / 
                               (double)sum_of_weights));
        }
    }
    printf("Numeric Score: %5.4f\n", numeric_score);
    printf("Points Penalty Per Day Late: %d\n", penalty_per_day);
    printf("Number of Assignments Dropped: %d\nValues Provided:\n", 
            num_to_drop);
    printf("Assignment, Score, Weight, Days Late\n");
    for (index = 0; index < num_of_assignments; index++) {
        printf("%d, %d, %d, %d\n", index+1, assignment_scores[index], 
                weight[index], late[index]);
    }
    if (c == 'y' || c == 'Y') {
        printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, 
                standard_deviation);
    }
}