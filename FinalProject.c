#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>

// Constants
#define MAX_USERS 10
#define MAX_BOOKS 20
#define MAX_WISHLIST 10
#define MAX_REVIEWS 5
#define MAX_TRANSACTIONS 50
#define MAX_INSTITUTIONS 5
#define MAX_AUCTIONS 5
#define MAX_EXCHANGE_BOOKS 10
#define ADMIN_PHONE "admin123"
#define ADMIN_PASSWORD "adminpass"

// User Structure
struct User {
    char emailOrPhone[50];
    char password[20];
    char dob[12];
    int wishlist[MAX_WISHLIST]; 
    float rating; 
    int ratingCount; 
    int booksBought; 
    char preferredCategories[3][30]; 
    char phoneNumber[12]; 
};

// Book Structure
struct Book {
    char name[50];
    char author[50];
    char condition[20];
    char category[30];
    float price;
    float minPrice;
    int isAvailable;
    int isForExchange; 
    char seller[50]; 
    int reviewCount;
    struct Review {
        int rating;  
        char comment[100];
    } reviews[MAX_REVIEWS];
    float historicalPrices[5];
    int priceCount;
};

// Exchange Book Structure
struct ExchangeBook {
    char name[50];
    char author[50];
    char condition[20];
    char category[30];
    float desiredPrice;
    char seller[50]; 
    char offeredBy[50]; 
    char offeredBookName[50]; 
    int isAvailable;
};

//Auction Structure
struct Auction {
    int bookIndex;
    float currentBid;
    float startingPrice;
    char highestBidder[50];
    time_t endTime;
    int isActive;
};
// Transaction Structure
struct Transaction {
    int bookIndex;
    int buyerIndex;
    int sellerIndex;
    float price;
    time_t date;
    int isDonation; 
    int isExchange;
    int completed; 
    char paymentNumber[15]; 
    char deliveryLocation[50];
};

// Institution Structure
struct Institution {
    char name[50];
    char address[100];
    char contact[15];
};

// Global Variables
struct User users[MAX_USERS];
struct Book books[MAX_BOOKS];
struct ExchangeBook exchangeBookList[MAX_EXCHANGE_BOOKS];
struct Auction auctions[MAX_AUCTIONS];
struct Transaction transactions[MAX_TRANSACTIONS];
struct Institution institutions[MAX_INSTITUTIONS];
int userCount = 0;
int bookCount = 0;
int exchangeBookCount = 0;
int auctionCount = 0;
int transactionCount = 0;
int institutionCount = 0;

// Function Prototypes
void registerUser();
int loginUser();
void sellerMenu(int userIndex);
void buyerMenu(int userIndex);
void adminMenu();
void sellBook(int userIndex);
void searchBooks();
void addToWishlist(int userIndex);
void viewWishlist(int userIndex);
void negotiatePrice(int userIndex, int bookIndex);
void makePayment(int userIndex, int bookIndex);
void deliveryOptions(int userIndex);
int validateGmailPassword(const char* password);
int validatePhonePassword(const char* password);
int validatePhoneNumber(const char* phoneNumber);
int isUserExists(const char* emailOrPhone);
int isDuplicateUser(const char* emailOrPhone);
void getPassword(char* password);
void giveReview(int bookIndex);
void displayReviews(int bookIndex);
void exchangeBooks(int userIndex);
void listBookForAuction(int userIndex);
void bidOnAuction(int userIndex);
void viewAuctions();
void viewMyAuctions(int userIndex);
void donateBook(int userIndex);
void addInstitution();
void recommendBooks(int userIndex);
void rateUser(int raterIndex, int ratedIndex);
void viewUserRating(int userIndex);
void updatePriceHistory(int bookIndex, float price);
void showPriceTrends();
void viewTransactions();
void completeTransaction(int transactionIndex);
void auctionMenu(int userIndex);
void viewAllBooks();
int validateGmail(const char* email);
void listBookForExchange(int userIndex);
void viewExchangeBooks();
void offerExchange(int userIndex);
void confirmExchange(int exchangeIndex);
void switchToBuyer(int userIndex);
void switchToSeller(int userIndex);
void forgotPassword();
void resetPassword(int userIndex);

// Helper function to find user index by email/phone
int findUserIndex(const char* emailOrPhone) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].emailOrPhone, emailOrPhone) == 0) {
            return i;
        }
    }
    return -1;
}

// Check if user already exists
int isUserExists(const char* emailOrPhone) {
    return findUserIndex(emailOrPhone) != -1;
}

// Check for duplicate user
int isDuplicateUser(const char* emailOrPhone) {
    return isUserExists(emailOrPhone);
}

// Validate Gmail format and check for uppercase
int validateGmail(const char* email) {
    if (strstr(email, "@gmail.com") == NULL) {
        return 0;
    }
    
    // Check for uppercase letters before @
    for (int i = 0; email[i] != '@'; i++) {
        if (isupper(email[i])) {
            return 0;
        }
    }
    return 1;
}

// Register User
void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }

    int choice;
    char emailOrPhone[50];

    printf("Choose Registration Method:\n");
    printf("1. Register with Phone Number\n2. Register with Gmail\nEnter choice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline

    if (choice == 1) {
        printf("Enter Phone Number (11 digits): ");
        scanf("%s", emailOrPhone);
        if (!validatePhoneNumber(emailOrPhone)) {
            printf("Invalid Phone Number! It must be 11 digits.\n");
            return;
        }
        // Store phone number separately for password recovery
        strcpy(users[userCount].phoneNumber, emailOrPhone);
    } else if (choice == 2) {
        printf("Enter Gmail Address: ");
        scanf("%s", emailOrPhone);
        if (!validateGmail(emailOrPhone)) {
            printf("Invalid Gmail format! Must be a valid Gmail address with no uppercase letters.\n");
            return;
        }
        // For gmail users, ask for phone number for password recovery
        printf("Enter Phone Number (11 digits) for password recovery: ");
        scanf("%s", users[userCount].phoneNumber);
        if (!validatePhoneNumber(users[userCount].phoneNumber)) {
            printf("Invalid Phone Number! It must be 11 digits.\n");
            return;
        }
    } else {
        printf("Invalid choice! Registration failed.\n");
        return;
    }

    if (isDuplicateUser(emailOrPhone)) {
        printf("Number/Gmail already exists!\n");
        return;
    }

    char password[20];
    printf("Enter Password : ");
    getPassword(password);

    if (strlen(password) < 8) {
        printf("\nPassword must be at least 8 characters long.\n");
        return;
    }

    if ((choice == 1 && !validatePhonePassword(password)) || (choice == 2 && !validateGmailPassword(password))) {
        printf("\nPassword must contain at least one uppercase letter and one special character.\n");
        return;
    }

    strcpy(users[userCount].emailOrPhone, emailOrPhone);
    strcpy(users[userCount].password, password);
    
    // Initialize new user fields
    users[userCount].rating = 0;
    users[userCount].ratingCount = 0;
    users[userCount].booksBought = 0;
    strcpy(users[userCount].preferredCategories[0], "");
    strcpy(users[userCount].preferredCategories[1], "");
    strcpy(users[userCount].preferredCategories[2], "");
    
    printf("\nEnter your date of birth (DD-MM-YYYY): ");
    scanf("%s", users[userCount].dob);
    
    userCount++;

    printf("\nRegistration Successful!\n");
}

// Hide password input
void getPassword(char* password) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter key
            password[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace key
            if (i > 0) {
                i--;
                printf("\b \b"); // Erase the character
            }
        } else if (i < 19) {
            password[i++] = ch;
            printf("*");
        }
    }
}

// Validate Gmail Password
int validateGmailPassword(const char* password) {
    int hasUppercase = 0;
    int hasSpecialChar = 0;

    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) hasUppercase = 1;
        if (!isalnum(password[i])) hasSpecialChar = 1;
    }

    return (hasUppercase && hasSpecialChar);
}

// Validate Phone Password
int validatePhonePassword(const char* password) {
    return validateGmailPassword(password);
}

// Validate Phone Number
int validatePhoneNumber(const char* phoneNumber) {
    if (strlen(phoneNumber) != 11) return 0;
    for (int i = 0; i < 11; i++) {
        if (!isdigit(phoneNumber[i])) return 0;
    }
    return 1;
}

// Forgot Password Feature
void forgotPassword() {
    char phoneNumber[12];
    char dob[12];
    
    printf("\n===== Password Recovery =====\n");
    printf("Enter your registered phone number: ");
    scanf("%s", phoneNumber);
    
    if (!validatePhoneNumber(phoneNumber)) {
        printf("Invalid phone number format!\n");
        return;
    }
    
    // Find user by phone number
    int userIndex = -1;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].phoneNumber, phoneNumber) == 0) {
            userIndex = i;
            break;
        }
    }
    
    if (userIndex == -1) {
        printf("No account found with this phone number!\n");
        return;
    }
    
    printf("Enter your date of birth (DD-MM-YYYY): ");
    scanf("%s", dob);
    
    if (strcmp(users[userIndex].dob, dob) != 0) {
        printf("Date of birth doesn't match our records!\n");
        return;
    }
    
    // Allow password reset
    resetPassword(userIndex);
}

// Reset Password
void resetPassword(int userIndex) {
    char newPassword[20];
    char confirmPassword[20];
    
    printf("\nEnter new password: ");
    getPassword(newPassword);
    
    printf("\nConfirm new password: ");
    getPassword(confirmPassword);
    
    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("\nPasswords don't match!\n");
        return;
    }
    
    if (strlen(newPassword) < 8) {
        printf("\nPassword must be at least 8 characters long.\n");
        return;
    }
    
    // Check password requirements based on registration method
    int isGmail = strstr(users[userIndex].emailOrPhone, "@gmail.com") != NULL;
    if ((isGmail && !validateGmailPassword(newPassword)) || 
        (!isGmail && !validatePhonePassword(newPassword))) {
        printf("\nPassword must contain at least one uppercase letter and one special character.\n");
        return;
    }
    
    strcpy(users[userIndex].password, newPassword);
    printf("\nPassword reset successfully!\n");
}

// Login User
int loginUser() {
    int choice;
    printf("\n===== Login =====\n");
    printf("1. Login with credentials\n2. Forgot password\nEnter choice: ");
    scanf("%d", &choice);
    
    if (choice == 2) {
        forgotPassword();
        return -1;
    }
    
    char emailOrPhone[50], password[20];

    printf("Enter Email or Phone Number: ");
    scanf("%s", emailOrPhone);
    printf("Enter Password: ");
    getPassword(password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].emailOrPhone, emailOrPhone) == 0 && strcmp(users[i].password, password) == 0) {
            printf("\nLogin Successful!\n");
            
            // Ask user to select their role
            int role;
            printf("\nSelect your role:\n");
            printf("1. Seller\n2. Buyer\nEnter choice: ");
            scanf("%d", &role);
            
            if (role == 1) {
                sellerMenu(i);
            } else if (role == 2) {
                buyerMenu(i);
            } else {
                printf("Invalid role selection!\n");
                return -1;
            }
            
            return i;
        }
    }
    printf("\nInvalid credentials!\n");
    return -1;
}

// Admin Login
void adminMenu() {
    char phone[50], password[20];
    printf("\n===== Admin Login =====\n");
    printf("Enter Admin Phone: ");
    scanf("%s", phone);
    printf("Enter Admin Password: ");
    getPassword(password);

    if (strcmp(phone, ADMIN_PHONE) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("\nAdmin Login Successful!\n");
        int choice;
        while (1) {
            printf("\n===== Admin Menu =====\n");
            printf("1. View Transactions\n2. Remove Book\n3. Add Institution\n4. Logout\nEnter choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1: viewTransactions(); break;
                case 2: {
                    int bookIndex;
                    printf("Enter Book Index to Remove (1 to %d): ", bookCount);
                    scanf("%d", &bookIndex);
                    if (bookIndex >= 1 && bookIndex <= bookCount) {
                        books[bookIndex-1].isAvailable = 0; // Mark book as removed
                        printf("Book Removed Successfully.\n");
                    } else {
                        printf("Invalid Book Index.\n");
                    }
                    break;
                }
                case 3: addInstitution(); break;
                case 4: return;
                default: printf("Invalid choice! Try again.\n");
            }
        }
    } else {
        printf("\nInvalid Admin Credentials!\n");
    }
}

// View All Books (categorized)
void viewAllBooks() {
    printf("\n===== All Available Books =====\n");
    
    // Books for sale
    printf("\n===== Books for Sale =====\n");
    int found = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isAvailable && !books[i].isForExchange) {
            printf("%d. %s by %s (Price: %.2f Taka, Condition: %s, Category: %s)\n", 
                  i+1, books[i].name, books[i].author, books[i].price, 
                  books[i].condition, books[i].category);
            found = 1;
        }
    }
    if (!found) printf("No books available for sale.\n");
    
    // Books for exchange
    printf("\n===== Books for Exchange =====\n");
    found = 0;
    for (int i = 0; i < exchangeBookCount; i++) {
        if (exchangeBookList[i].isAvailable) {
            printf("%d. %s by %s (Desired Price: %.2f Taka, Condition: %s, Category: %s)\n", 
                  i+1, exchangeBookList[i].name, exchangeBookList[i].author, 
                  exchangeBookList[i].desiredPrice, exchangeBookList[i].condition, 
                  exchangeBookList[i].category);
            found = 1;
        }
    }
    if (!found) printf("No books available for exchange.\n");
    
    // Books in auction
    printf("\n===== Books in Auction =====\n");
    found = 0;
    time_t now = time(NULL);
    for (int i = 0; i < auctionCount; i++) {
        if (auctions[i].isActive && now < auctions[i].endTime) {
            int bookIndex = auctions[i].bookIndex;
            printf("%d. %s by %s (Current Bid: %.2f Taka, Time Remaining: %ld seconds)\n", 
                  i+1, books[bookIndex].name, books[bookIndex].author, 
                  auctions[i].currentBid, auctions[i].endTime - now);
            found = 1;
        }
    }
    if (!found) printf("No active auctions available.\n");
}

// Switch to Buyer from Seller Menu
void switchToBuyer(int userIndex) {
    printf("\nSwitching to Buyer Mode...\n");
    buyerMenu(userIndex);
}

// Switch to Seller from Buyer Menu
void switchToSeller(int userIndex) {
    printf("\nSwitching to Seller Mode...\n");
    sellerMenu(userIndex);
}

// Seller Menu
void sellerMenu(int userIndex) {
    int choice, bookIndex;
    while (1) {
        printf("\n===== Seller Menu =====\n");
        printf("1. Sell Books\n2. View My Listings\n3. Exchange Books\n");
        printf("4. List Book for Auction\n5. View My Auctions\n6. Donate Books\n");
        printf("7. View My Rating\n8. View All Books\n9. Act as Buyer\n10. Logout\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: sellBook(userIndex); break;
            case 2: {
                printf("\n===== My Listings =====\n");
                int found = 0;
                for (int i = 0; i < bookCount; i++) {
                    if (strcmp(books[i].seller, users[userIndex].emailOrPhone) == 0) {
                        printf("%d. %s by %s (Price: %.2f Taka, Status: %s)\n", 
                              i+1, books[i].name, books[i].author, books[i].price,
                              books[i].isAvailable ? "Available" : "Sold");
                        found = 1;
                    }
                }
                if (!found) printf("You have no active listings.\n");
                break;
            }
            case 3: exchangeBooks(userIndex); break;
            case 4: listBookForAuction(userIndex); break;
            case 5: viewMyAuctions(userIndex); break;
            case 6: donateBook(userIndex); break;
            case 7: viewUserRating(userIndex); break;
            case 8: viewAllBooks(); break;
            case 9: switchToBuyer(userIndex); return;
            case 10: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

// Buyer Menu
void buyerMenu(int userIndex) {
    int choice, bookIndex;
    while (1) {
        printf("\n===== Buyer Menu =====\n");
        printf("1. Search Books\n2. Add to Wishlist\n3. View Wishlist\n");
        printf("4. Negotiate Price\n5. Make Payment\n6. Delivery Method\n");
        printf("7. Give Review\n8. Exchange Books\n9. Bid on Auction\n");
        printf("10. View Recommendations\n11. Rate User\n12. View My Rating\n");
        printf("13. View Price Trends\n14. View All Books\n15. Act as Seller\n16. Logout\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: searchBooks(); break;
            case 2: addToWishlist(userIndex); break;
            case 3: viewWishlist(userIndex); break;
            case 4: 
                printf("Enter Book Index: ");
                scanf("%d", &bookIndex);
                negotiatePrice(userIndex, bookIndex-1); 
                break;
            case 5: 
                printf("Enter Book Index: ");
                scanf("%d", &bookIndex);
                makePayment(userIndex, bookIndex-1); 
                break;
            case 6: deliveryOptions(userIndex); break;
            case 7: 
                printf("Enter Book Index: ");
                scanf("%d", &bookIndex);
                giveReview(bookIndex-1);
                break;
            case 8: exchangeBooks(userIndex); break;
            case 9: bidOnAuction(userIndex); break;
            case 10: recommendBooks(userIndex); break;
            case 11: 
                printf("Enter User Index to Rate: ");
                scanf("%d", &bookIndex);
                rateUser(userIndex, bookIndex-1);
                break;
            case 12: viewUserRating(userIndex); break;
            case 13: showPriceTrends(); break;
            case 14: viewAllBooks(); break;
            case 15: switchToSeller(userIndex); return;
            case 16: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

// Sell Books
void sellBook(int userIndex) {
    if (bookCount >= MAX_BOOKS) {
        printf("Book limit reached!\n");
        return;
    }

    printf("\nEnter Book Details:\n");
    printf("Enter Book Name: ");
    scanf(" %[^\n]", books[bookCount].name);
    printf("Enter Author Name: ");
    scanf(" %[^\n]", books[bookCount].author);
    printf("Enter Book Condition (New/Good/Acceptable): ");
    scanf("%s", books[bookCount].condition);
    printf("Enter Book Category (Fiction/Non-Fiction/Academic/Science): ");
    scanf("%s", books[bookCount].category);
    printf("Enter Book Price (in Taka): ");
    scanf("%f", &books[bookCount].price);
    printf("Enter Minimum Negotiable Price (in Taka): ");
    scanf("%f", &books[bookCount].minPrice);

    books[bookCount].isAvailable = 1;
    books[bookCount].isForExchange = 0; // Default to not for exchange
    strcpy(books[bookCount].seller, users[userIndex].emailOrPhone);
    books[bookCount].reviewCount = 0;
    books[bookCount].priceCount = 0;
    updatePriceHistory(bookCount, books[bookCount].price);
    
    printf("Is this book available for exchange? (1=Yes, 0=No): ");
    scanf("%d", &books[bookCount].isForExchange);

    printf("Book Listed Successfully! (Index: %d)\n", bookCount+1);
    bookCount++;
}

// Search Books (Modified to Display Reviews)
void searchBooks() {
    char searchTerm[50];
    printf("Enter Book Name or Author to Search: ");
    scanf(" %[^\n]", searchTerm);

    printf("\n===== Search Results =====\n");
    int found = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isAvailable && 
            (strstr(books[i].name, searchTerm) != NULL || 
             strstr(books[i].author, searchTerm) != NULL)) {
            printf("Book %d: %s by %s (Condition: %s, Price: %.2f Taka)\n", 
                i+1, books[i].name, books[i].author, books[i].condition, books[i].price);
            displayReviews(i);
            found = 1;
        }
    }
    if (!found) {
        printf("No books found matching your search.\n");
    }
}

// Add to Wishlist
void addToWishlist(int userIndex) {
    int bookIndex;
    printf("Enter Book Index to Add to Wishlist (1 to %d): ", bookCount);
    scanf("%d", &bookIndex);

    if (bookIndex >= 1 && bookIndex <= bookCount && books[bookIndex-1].isAvailable == 1) {
        users[userIndex].wishlist[bookIndex-1] = 1;
        
        // Update user preferences based on book category
        for (int i = 0; i < 3; i++) {
            if (strcmp(users[userIndex].preferredCategories[i], books[bookIndex-1].category) == 0) {
                break; // Already in preferences
            }
            if (strcmp(users[userIndex].preferredCategories[i], "") == 0) {
                strcpy(users[userIndex].preferredCategories[i], books[bookIndex-1].category);
                break;
            }
        }
        
        printf("Book added to wishlist!\n");
    } else {
        printf("Invalid book or book not available.\n");
    }
}

// View Wishlist
void viewWishlist(int userIndex) {
    printf("\nYour Wishlist:\n");
    int found = 0;
    for (int i = 0; i < bookCount; i++) {
        if (users[userIndex].wishlist[i] == 1) {
            printf("%d. %s by %s (Price: %.2f Taka)\n", i+1, books[i].name, books[i].author, books[i].price);
            found = 1;
        }
    }
    if (!found) {
        printf("Your wishlist is empty.\n");
    }
}

// Negotiate Price
void negotiatePrice(int userIndex, int bookIndex) {
    if (bookIndex < 0 || bookIndex >= bookCount || !books[bookIndex].isAvailable) {
        printf("Invalid book selection!\n");
        return;
    }

    float offeredPrice;
    printf("Current Price: %.2f Taka, Minimum Price: %.2f Taka\n", books[bookIndex].price, books[bookIndex].minPrice);
    printf("Enter your offered price (in Taka): ");
    scanf("%f", &offeredPrice);

    if (offeredPrice >= books[bookIndex].minPrice) {
        printf("Price Accepted! Proceed to Payment.\n");
        makePayment(userIndex, bookIndex);
    } else {
        printf("Your offer is too low. Try again later.\n");
    }
}

// Make Payment (Updated with payment number)
void makePayment(int userIndex, int bookIndex) {
    if (bookIndex < 0 || bookIndex >= bookCount || !books[bookIndex].isAvailable) {
        printf("Invalid book selection!\n");
        return;
    }

    int paymentMethod;
    printf("\nChoose Payment Method:\n");
    printf("1. Mobile Banking\n2. Bank Transfer\n3. Cash on Delivery\nEnter choice: ");
    scanf("%d", &paymentMethod);

    char paymentNumber[15] = "";
    if (paymentMethod == 1 || paymentMethod == 2) {
        printf("Enter your mobile/bank account number: ");
        scanf("%s", paymentNumber);
    }

    switch (paymentMethod) {
        case 1: 
            printf("You chose Mobile Banking with number: %s\n", paymentNumber); 
            break;
        case 2: 
            printf("You chose Bank Transfer with account: %s\n", paymentNumber); 
            break;
        case 3: 
            printf("You chose Cash on Delivery.\n"); 
            break;
        default: 
            printf("Invalid choice! Payment cancelled.\n"); 
            return;
    }

    // Record transaction
    if (transactionCount < MAX_TRANSACTIONS) {
        transactions[transactionCount].bookIndex = bookIndex;
        transactions[transactionCount].buyerIndex = userIndex;
        
        // Find seller index
        int sellerIndex = findUserIndex(books[bookIndex].seller);
        transactions[transactionCount].sellerIndex = sellerIndex;
        
        transactions[transactionCount].price = books[bookIndex].price;
        transactions[transactionCount].date = time(NULL);
        transactions[transactionCount].isDonation = 0;
        transactions[transactionCount].isExchange = 0;
        transactions[transactionCount].completed = 1;
        strcpy(transactions[transactionCount].paymentNumber, paymentNumber);
        strcpy(transactions[transactionCount].deliveryLocation, "");
        transactionCount++;
        
        // Update user's books bought count
        users[userIndex].booksBought++;
        
        // Mark book as sold
        books[bookIndex].isAvailable = 0;
        
        printf("Payment Successful! Order Confirmed.\n");
    } else {
        printf("Transaction limit reached. Please try again later.\n");
    }
}

// Delivery Options (Updated with meetup location)
void deliveryOptions(int userIndex) {
    int choice;
    printf("\nSelect Delivery Method:\n");
    printf("1. Meetup Point\n2. Courier Service\n3. Cancel\nEnter choice: ");
    scanf("%d", &choice);

    char location[50] = "";
    if (choice == 1) {
        printf("Enter meetup location: ");
        scanf(" %[^\n]", location);
    }

    switch (choice) {
        case 1: 
            printf("You chose Meetup Point at %s.\n", location); 
            // Store delivery location in the most recent transaction
            if (transactionCount > 0) {
                strcpy(transactions[transactionCount-1].deliveryLocation, location);
            }
            break;
        case 2: 
            printf("You chose Courier Service.\n"); 
            break;
        case 3: 
            printf("Delivery cancelled.\n"); 
            break;
        default: 
            printf("Invalid choice! Delivery cancelled.\n");
    }
}

// Give a Review
void giveReview(int bookIndex) {
    if (bookIndex < 0 || bookIndex >= bookCount) {
        printf("Invalid book selection!\n");
        return;
    }

    if (books[bookIndex].reviewCount >= MAX_REVIEWS) {
        printf("This book has reached the maximum number of reviews!\n");
        return;
    }

    int rating;
    char comment[100];

    printf("Enter rating (1-5): ");
    scanf("%d", &rating);
    if (rating < 1 || rating > 5) {
        printf("Invalid rating! Must be between 1 and 5.\n");
        return;
    }

    printf("Enter your review: ");
    scanf(" %[^\n]", comment);

    int reviewIndex = books[bookIndex].reviewCount;
    books[bookIndex].reviews[reviewIndex].rating = rating;
    strcpy(books[bookIndex].reviews[reviewIndex].comment, comment);
    books[bookIndex].reviewCount++;

    printf("Review added successfully!\n");
}

// Display Reviews for a Book
void displayReviews(int bookIndex) {
    if (books[bookIndex].reviewCount == 0) {
        printf("No reviews yet.\n");
        return;
    }

    printf("Reviews for %s:\n", books[bookIndex].name);
    for (int i = 0; i < books[bookIndex].reviewCount; i++) {
        printf("- Rating: %d/5\n", books[bookIndex].reviews[i].rating);
        printf("  Comment: %s\n", books[bookIndex].reviews[i].comment);
    }
}

// List Book for Exchange
void listBookForExchange(int userIndex) {
    if (exchangeBookCount >= MAX_EXCHANGE_BOOKS) {
        printf("Exchange book limit reached!\n");
        return;
    }

    printf("\nEnter Book Details for Exchange:\n");
    printf("Enter Book Name: ");
    scanf(" %[^\n]", exchangeBookList[exchangeBookCount].name);
    printf("Enter Author Name: ");
    scanf(" %[^\n]", exchangeBookList[exchangeBookCount].author);
    printf("Enter Book Condition (New/Good/Acceptable): ");
    scanf("%s", exchangeBookList[exchangeBookCount].condition);
    printf("Enter Book Category (Fiction/Non-Fiction/Academic/Science): ");
    scanf("%s", exchangeBookList[exchangeBookCount].category);
    printf("Enter Desired Price (in Taka): ");
    scanf("%f", &exchangeBookList[exchangeBookCount].desiredPrice);

    exchangeBookList[exchangeBookCount].isAvailable = 1;
    strcpy(exchangeBookList[exchangeBookCount].seller, users[userIndex].emailOrPhone);
    strcpy(exchangeBookList[exchangeBookCount].offeredBy, "");
    strcpy(exchangeBookList[exchangeBookCount].offeredBookName, "");

    printf("Book listed for exchange successfully! (Index: %d)\n", exchangeBookCount+1);
    exchangeBookCount++;
}

// View Exchange Books
void viewExchangeBooks() {
    printf("\n===== Books Available for Exchange =====\n");
    int found = 0;
    for (int i = 0; i < exchangeBookCount; i++) {
        if (exchangeBookList[i].isAvailable) {
            printf("%d. %s by %s (Desired Price: %.2f Taka, Condition: %s)\n", 
                  i+1, exchangeBookList[i].name, exchangeBookList[i].author, 
                  exchangeBookList[i].desiredPrice, exchangeBookList[i].condition);
            if (strlen(exchangeBookList[i].offeredBy) > 0) {
                printf("   Offered Book: %s by %s\n", exchangeBookList[i].offeredBookName, 
                      exchangeBookList[i].offeredBy);
            }
            found = 1;
        }
    }
    if (!found) printf("No books available for exchange.\n");
}

// Offer Exchange
void offerExchange(int userIndex) {
    viewExchangeBooks();
    
    printf("Enter the index of the book you want to exchange for (1 to %d, or 0 to cancel): ", exchangeBookCount);
    int exchangeIndex;
    scanf("%d", &exchangeIndex);
    
    if (exchangeIndex == 0) return;
    if (exchangeIndex < 1 || exchangeIndex > exchangeBookCount) {
        printf("Invalid book index!\n");
        return;
    }
    
    exchangeIndex--; // Convert to 0-based index
    
    if (strcmp(exchangeBookList[exchangeIndex].seller, users[userIndex].emailOrPhone) == 0) {
        printf("You cannot exchange with yourself!\n");
        return;
    }
    
    if (!exchangeBookList[exchangeIndex].isAvailable) {
        printf("This book is not available for exchange!\n");
        return;
    }
    
    if (strlen(exchangeBookList[exchangeIndex].offeredBy) > 0) {
        printf("An offer is already pending for this book.\n");
        return;
    }
    
    printf("Enter the name of the book you're offering: ");
    char offeredBook[50];
    scanf(" %[^\n]", offeredBook);
    
    // Record the offer
    strcpy(exchangeBookList[exchangeIndex].offeredBy, users[userIndex].emailOrPhone);
    strcpy(exchangeBookList[exchangeIndex].offeredBookName, offeredBook);
    
    printf("Exchange offer submitted! Waiting for confirmation from the owner.\n");
}

// Confirm Exchange
void confirmExchange(int exchangeIndex) {
    if (exchangeIndex < 0 || exchangeIndex >= exchangeBookCount) {
        printf("Invalid exchange index!\n");
        return;
    }
    
    if (!exchangeBookList[exchangeIndex].isAvailable) {
        printf("This exchange is no longer available!\n");
        return;
    }
    
    if (strlen(exchangeBookList[exchangeIndex].offeredBy) == 0) {
        printf("No offer to confirm!\n");
        return;
    }
    
    printf("Confirm exchange of %s for %s (offered by %s)? (1=Yes, 0=No): ",
          exchangeBookList[exchangeIndex].name, exchangeBookList[exchangeIndex].offeredBookName,
          exchangeBookList[exchangeIndex].offeredBy);
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1) {
        // Create exchange transaction
        if (transactionCount < MAX_TRANSACTIONS) {
            transactions[transactionCount].bookIndex = -1; // No book index for exchange
            transactions[transactionCount].buyerIndex = findUserIndex(exchangeBookList[exchangeIndex].offeredBy);
            transactions[transactionCount].sellerIndex = findUserIndex(exchangeBookList[exchangeIndex].seller);
            transactions[transactionCount].price = exchangeBookList[exchangeIndex].desiredPrice;
            transactions[transactionCount].date = time(NULL);
            transactions[transactionCount].isDonation = 0;
            transactions[transactionCount].isExchange = 1;
            transactions[transactionCount].completed = 1;
            strcpy(transactions[transactionCount].paymentNumber, "");
            strcpy(transactions[transactionCount].deliveryLocation, "");
            transactionCount++;
            
            // Mark exchange as completed
            exchangeBookList[exchangeIndex].isAvailable = 0;
            
            printf("Exchange confirmed successfully!\n");
        } else {
            printf("Transaction limit reached. Please try again later.\n");
        }
    } else {
        // Reject the offer
        strcpy(exchangeBookList[exchangeIndex].offeredBy, "");
        strcpy(exchangeBookList[exchangeIndex].offeredBookName, "");
        printf("Exchange offer rejected.\n");
    }
}

// Exchange Books Feature
void exchangeBooks(int userIndex) {
    int choice;
    while (1) {
        printf("\n===== Book Exchange =====\n");
        printf("1. List Book for Exchange\n2. View Available Books for Exchange\n");
        printf("3. Offer Exchange\n4. View My Exchange Offers\n5. Back to Seller Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: listBookForExchange(userIndex); break;
            case 2: viewExchangeBooks(); break;
            case 3: offerExchange(userIndex); break;
            case 4: {
                printf("\n===== My Exchange Offers =====\n");
                int found = 0;
                for (int i = 0; i < exchangeBookCount; i++) {
                    if (strcmp(exchangeBookList[i].seller, users[userIndex].emailOrPhone) == 0 && 
                        strlen(exchangeBookList[i].offeredBy) > 0) {
                        printf("%d. %s by %s (Offered: %s by %s)\n", 
                              i+1, exchangeBookList[i].name, exchangeBookList[i].author,
                              exchangeBookList[i].offeredBookName, exchangeBookList[i].offeredBy);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("You have no exchange offers.\n");
                    break;
                }
                
                printf("Enter exchange index to confirm (or 0 to cancel): ");
                int exchangeIndex;
                scanf("%d", &exchangeIndex);
                if (exchangeIndex > 0) {
                    confirmExchange(exchangeIndex-1);
                }
                break;
            }
            case 5: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// List Book for Auction
void listBookForAuction(int userIndex) {
    if (auctionCount >= MAX_AUCTIONS) {
        printf("Auction limit reached!\n");
        return;
    }

    printf("Your Books Available for Auction:\n");
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isAvailable && strcmp(users[userIndex].emailOrPhone, books[i].seller) == 0) {
            printf("%d. %s by %s\n", i+1, books[i].name, books[i].author);
        }
    }

    printf("Enter book index to auction (1 to %d, or 0 to cancel): ", bookCount);
    int bookIndex;
    scanf("%d", &bookIndex);
    if (bookIndex == 0) return;
    bookIndex--; // Convert to 0-based index

    printf("Enter starting price (in Taka): ");
    float startPrice;
    scanf("%f", &startPrice);

    printf("Enter auction duration in hours: ");
    int hours;
    scanf("%d", &hours);

    auctions[auctionCount].bookIndex = bookIndex;
    auctions[auctionCount].currentBid = startPrice;
    auctions[auctionCount].startingPrice = startPrice;
    strcpy(auctions[auctionCount].highestBidder, "");
    auctions[auctionCount].endTime = time(NULL) + (hours * 3600);
    auctions[auctionCount].isActive = 1;
    auctionCount++;

    books[bookIndex].isAvailable = 0; // Remove from regular listings
    printf("Auction created successfully! (Auction Index: %d)\n", auctionCount);
}

// View Active Auctions
void viewAuctions() {
    printf("\n===== Active Auctions =====\n");
    time_t now = time(NULL);
    int found = 0;
    
    for (int i = 0; i < auctionCount; i++) {
        if (auctions[i].isActive) {
            if (now >= auctions[i].endTime) {
                auctions[i].isActive = 0; // Auction ended
                continue;
            }
            
            int bookIndex = auctions[i].bookIndex;
            printf("Auction %d: %s by %s\n", i+1, books[bookIndex].name, books[bookIndex].author);
            printf("Current Bid: %.2f Taka", auctions[i].currentBid);
            if (strlen(auctions[i].highestBidder) > 0) {
                printf(" (Highest Bidder: %s)", auctions[i].highestBidder);
            }
            printf("\nTime Remaining: %ld seconds\n", auctions[i].endTime - now);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No active auctions available.\n");
    }
}

// Bid on Auction
void bidOnAuction(int userIndex) {
    viewAuctions();
    
    printf("Enter auction index to bid on (1 to %d, or 0 to cancel): ", auctionCount);
    int auctionIndex;
    scanf("%d", &auctionIndex);
    if (auctionIndex == 0) return;
    auctionIndex--; // Convert to 0-based index
    
    if (auctionIndex < 0 || auctionIndex >= auctionCount || !auctions[auctionIndex].isActive) {
        printf("Invalid auction selection!\n");
        return;
    }
    
    time_t now = time(NULL);
    if (now >= auctions[auctionIndex].endTime) {
        auctions[auctionIndex].isActive = 0;
        printf("This auction has ended.\n");
        return;
    }
    
    float bidAmount;
    printf("Current bid: %.2f Taka\n", auctions[auctionIndex].currentBid);
    printf("Enter your bid amount (in Taka): ");
    scanf("%f", &bidAmount);
    
    if (bidAmount <= auctions[auctionIndex].currentBid) {
        printf("Your bid must be higher than the current bid!\n");
        return;
    }
    
    auctions[auctionIndex].currentBid = bidAmount;
    strcpy(auctions[auctionIndex].highestBidder, users[userIndex].emailOrPhone);
    printf("Bid placed successfully!\n");
}

// View My Auctions
void viewMyAuctions(int userIndex) {
    printf("\n===== My Auctions =====\n");
    time_t now = time(NULL);
    int found = 0;
    
    for (int i = 0; i < auctionCount; i++) {
        int bookIndex = auctions[i].bookIndex;
        if (strcmp(books[bookIndex].seller, users[userIndex].emailOrPhone) == 0) {
            printf("Auction %d: %s by %s\n", i+1, books[bookIndex].name, books[bookIndex].author);
            printf("Status: %s\n", auctions[i].isActive ? "Active" : "Ended");
            printf("Current Bid: %.2f Taka", auctions[i].currentBid);
            if (strlen(auctions[i].highestBidder) > 0) {
                printf(" (Highest Bidder: %s)", auctions[i].highestBidder);
            }
            printf("\n");
            found = 1;
        }
    }
    
    if (!found) {
        printf("You have no auctions.\n");
    }
}

// Donate Book Feature
void donateBook(int userIndex) {
    printf("\n===== Book Donation =====\n");
    printf("Your Available Books for Donation:\n");
    int hasBooks = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isAvailable && strcmp(users[userIndex].emailOrPhone, books[i].seller) == 0) {
            printf("%d. %s by %s\n", i+1, books[i].name, books[i].author);
            hasBooks = 1;
        }
    }
    
    if (!hasBooks) {
        printf("You have no books available to donate.\n");
        return;
    }

    printf("Enter book index to donate (1 to %d, or 0 to cancel): ", bookCount);
    int bookIndex;
    scanf("%d", &bookIndex);
    if (bookIndex == 0) return;
    bookIndex--; // Convert to 0-based index

    printf("\nAvailable Institutions:\n");
    for (int i = 0; i < institutionCount; i++) {
        printf("%d. %s (%s)\n", i+1, institutions[i].name, institutions[i].contact);
    }

    printf("Select institution (1 to %d): ", institutionCount);
    int instIndex;
    scanf("%d", &instIndex);
    instIndex--; // Convert to 0-based index

    // Create donation transaction
    if (transactionCount < MAX_TRANSACTIONS) {
        transactions[transactionCount].bookIndex = bookIndex;
        transactions[transactionCount].buyerIndex = -1; // No buyer for donations
        transactions[transactionCount].sellerIndex = userIndex;
        transactions[transactionCount].price = 0;
        transactions[transactionCount].date = time(NULL);
        transactions[transactionCount].isDonation = 1;
        transactions[transactionCount].isExchange = 0;
        transactions[transactionCount].completed = 1;
        strcpy(transactions[transactionCount].paymentNumber, "");
        strcpy(transactions[transactionCount].deliveryLocation, "");
        transactionCount++;
        
        books[bookIndex].isAvailable = 0;
        printf("Donation arranged! Thank you for your generosity.\n");
    } else {
        printf("Transaction limit reached. Please try again later.\n");
    }
}

// Add Institution (Admin Only)
void addInstitution() {
    if (institutionCount >= MAX_INSTITUTIONS) {
        printf("Institution limit reached!\n");
        return;
    }

    printf("\nEnter Institution Details:\n");
    printf("Name: ");
    scanf(" %[^\n]", institutions[institutionCount].name);
    printf("Address: ");
    scanf(" %[^\n]", institutions[institutionCount].address);
    printf("Contact: ");
    scanf(" %[^\n]", institutions[institutionCount].contact);

    institutionCount++;
    printf("Institution added successfully!\n");
}

// Recommendation System
void recommendBooks(int userIndex) {
    printf("\n===== Recommended Books =====\n");
    printf("Based on your preferences, we recommend:\n");

    int found = 0;
    // Simple category-based recommendation
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isAvailable) {
            // Check if book matches user's preferred categories
            for (int j = 0; j < 3; j++) {
                if (strcmp(users[userIndex].preferredCategories[j], books[i].category) == 0) {
                    printf("- %s by %s (Category: %s, Price: %.2f Taka)\n", 
                          books[i].name, books[i].author, books[i].category, books[i].price);
                    found = 1;
                    break;
                }
            }
        }
    }
    
    if (!found) {
        printf("No recommendations available. Try adding books to your wishlist first.\n");
    }
}

// User Rating System
void rateUser(int raterIndex, int ratedIndex) {
    if (ratedIndex < 0 || ratedIndex >= userCount) {
        printf("Invalid user index!\n");
        return;
    }

    if (raterIndex == ratedIndex) {
        printf("You cannot rate yourself!\n");
        return;
    }

    printf("Rating user: %s\n", users[ratedIndex].emailOrPhone);
    printf("Enter rating (1-5): ");
    int rating;
    scanf("%d", &rating);

    if (rating < 1 || rating > 5) {
        printf("Invalid rating!\n");
        return;
    }

    // Update user rating
    float total = users[ratedIndex].rating * users[ratedIndex].ratingCount;
    users[ratedIndex].ratingCount++;
    users[ratedIndex].rating = (total + rating) / users[ratedIndex].ratingCount;

    printf("Thank you for your rating!\n");
}

// View User Rating
void viewUserRating(int userIndex) {
    printf("\n===== Your Rating =====\n");
    if (users[userIndex].ratingCount == 0) {
        printf("You have no ratings yet.\n");
    } else {
        printf("Your current rating: %.1f/5 (%d ratings)\n", 
              users[userIndex].rating, users[userIndex].ratingCount);
    }
}

// Price History Tracking
void updatePriceHistory(int bookIndex, float price) {
    if (bookIndex < 0 || bookIndex >= bookCount) return;
    
    if (books[bookIndex].priceCount < 5) {
        books[bookIndex].historicalPrices[books[bookIndex].priceCount++] = price;
    } else {
        // Shift array to make room for new price
        for (int i = 0; i < 4; i++) {
            books[bookIndex].historicalPrices[i] = books[bookIndex].historicalPrices[i+1];
        }
        books[bookIndex].historicalPrices[4] = price;
    }
}

// Show Price Trends
void showPriceTrends() {
    printf("\n===== Price Trends =====\n");
    printf("Select a book to view price history:\n");
    
    for (int i = 0; i < bookCount; i++) {
        printf("%d. %s by %s\n", i+1, books[i].name, books[i].author);
    }
    
    printf("Enter book index (1 to %d, or 0 to cancel): ", bookCount);
    int bookIndex;
    scanf("%d", &bookIndex);
    if (bookIndex == 0) return;
    bookIndex--; // Convert to 0-based index
    
    if (bookIndex < 0 || bookIndex >= bookCount) {
        printf("Invalid book index!\n");
        return;
    }
    
    if (books[bookIndex].priceCount == 0) {
        printf("No price history available for this book.\n");
        return;
    }
    
    printf("\nPrice History for %s (in Taka):\n", books[bookIndex].name);
    for (int i = 0; i < books[bookIndex].priceCount; i++) {
        printf("- %.2f\n", books[bookIndex].historicalPrices[i]);
    }
}

// View Transactions (Admin Only)
void viewTransactions() {
    printf("\n===== Transaction History =====\n");
    for (int i = 0; i < transactionCount; i++) {
        printf("Transaction %d:\n", i+1);
        
        if (transactions[i].bookIndex >= 0) {
            printf("Book: %s by %s\n", 
                  books[transactions[i].bookIndex].name, 
                  books[transactions[i].bookIndex].author);
        } else {
            printf("Book: Exchange Transaction\n");
        }
        
        if (transactions[i].isDonation) {
            printf("Type: Donation to %s\n", institutions[0].name); // Simplified
        } else if (transactions[i].isExchange) {
            printf("Type: Book Exchange\n");
        } else {
            printf("Type: Purchase\n");
        }
        
        printf("Amount: %.2f Taka\n", transactions[i].price);
        printf("Payment Number: %s\n", transactions[i].paymentNumber);
        printf("Delivery Location: %s\n", transactions[i].deliveryLocation);
        printf("Date: %s", ctime(&transactions[i].date));
        printf("Status: %s\n", transactions[i].completed ? "Completed" : "Pending");
        printf("\n");
    }
}

// Complete Transaction (Admin Only)
void completeTransaction(int transactionIndex) {
    if (transactionIndex < 0 || transactionIndex >= transactionCount) {
        printf("Invalid transaction index!\n");
        return;
    }
    
    if (transactions[transactionIndex].completed) {
        printf("Transaction already completed!\n");
        return;
    }
    
    transactions[transactionIndex].completed = 1;
    printf("Transaction marked as completed.\n");
}

int main() {
    // Initialize some institutions
    strcpy(institutions[0].name, "City Public Library");
    strcpy(institutions[0].address, "123 Main St");
    strcpy(institutions[0].contact, "555-1234");
    institutionCount++;

    strcpy(institutions[1].name, "Local High School");
    strcpy(institutions[1].address, "456 Education Ave");
    strcpy(institutions[1].contact, "555-5678");
    institutionCount++;

    int userIndex;
    int choice;

    while (1) {
        printf("\n===== Old Book Recycle System =====\n");
        printf("1. Register\n2. Login\n3. Admin Login\n4. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: registerUser(); break;
            case 2: 
                userIndex = loginUser();
                break;
            case 3: adminMenu(); break;
            case 4: return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}