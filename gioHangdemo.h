#ifndef GIO_HANG_H
#define GIO_HANG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <utility>

using namespace std;

// ============================================================================
// PHAN 1: KHUNG CHUONG TRINH - HANG SO & CAU TRUC DU LIEU (DATA STRUCTURES)
// ============================================================================

const int MAX_CATALOG = 5000;
const int HASH_SIZE = 11;

// 1.1. Cau truc San pham (Kho hang) & Mon hang trong gio
struct Product {
    int id;             // Ma san pham (ProductID)
    string name;        // Ten san pham
    double price;       // Don gia
    int stock;          // So luong ton kho
    double discount;    // Giam gia truc tiep tren san pham
};

struct CartItem {
    int productId;
    string name;
    double unitPrice;   // Don gia sau khi tru discount
    int quantity;
    double subtotal() const { return unitPrice * quantity; }
};

// 1.2. Cau truc Danh sach lien ket don (Quan ly Gio hang)
struct CartNode {
    CartItem data;
    CartNode* next;
};

struct CartLinkedList {
    CartNode* head;
};

// 1.3. Cau truc Ngan xep - Stack (Phuc vu thao tac UNDO)
enum ActionType { ACT_ADD, ACT_REMOVE, ACT_UPDATE };

struct UndoAction {
    ActionType type;
    CartItem itemState;
    int oldQuantity;
};

struct StackNode {
    UndoAction data;
    StackNode* next;
};

struct UndoStack {
    StackNode* top;
};

// 1.4. Cau truc Hang doi - Queue (Quan ly Don hang Checkout)
struct Order {
    int orderId;
    string customerName;
    double finalTotal;
};

struct QueueNode {
    Order data;
    QueueNode* next;
};

struct OrderQueue {
    QueueNode* front;
    QueueNode* rear;
};

// 1.5. Cau truc Cay nhi phan tim kiem can bang (AVL Tree)
struct AVLNode {
    Product data;
    AVLNode* left;
    AVLNode* right;
    int height;
};

struct AVLTree {
    AVLNode* root;
};

// 1.6. Cau truc Bang bam - Hash Table (Quan ly Voucher)
struct VoucherNode {
    int voucherKey;
    string codeName;
    double discountValue;
    VoucherNode* next;
};

struct VoucherHashTable {
    VoucherNode* buckets[HASH_SIZE];
};

// ============================================================================
// PHAN 2: HE THONG MENU DIEU HUONG (MENU UI)
// ============================================================================

void showMainMenu(const string& csvSortedFile);
void showSortMenu();
void showSearchMenu();
void showCartSubMenu();

// ============================================================================
// PHAN 3: NHAP LIEU & DOC/GHI FILE (INPUT & FILE CSV I/O)
// ============================================================================

// 3.1. Nhap lieu tu ban phim (Console Input co kiem tra hop le)
int inputInt(const string& prompt);
string inputString(const string& prompt);

// 3.2. Nhap/Xuat du lieu tu file CSV
void initSampleCSV(const string& filename);
int loadCatalogFromCSV(const string& filename, Product catalog[], int maxCap, AVLTree& avlIndex);
bool saveCatalogToCSV(const string& filename, Product catalog[], int n);

// ============================================================================
// PHAN 4: CAC HAM DUNG CHUNG (SHARED / UTILITY FUNCTIONS)
// ============================================================================

// 4.1. Ham hien thi dung chung (Display Helpers)
void printCatalog(Product catalog[], int n, int previewLimit = 20);
void viewCart(CartLinkedList& cart, double voucherDiscount);
void displayVouchers(VoucherHashTable& ht);

// 4.2. Ham khoi tao, thao tac co ban & giai phong DSLK Gio hang
void initCart(CartLinkedList& list);
CartNode* createCartNode(CartItem x);
void insertTailCart(CartLinkedList& list, CartItem x);
CartNode* findCartNode(CartLinkedList& list, int productId);
void deleteHeadCart(CartLinkedList& list);
bool removeProductFromCart(CartLinkedList& list, int productId, CartItem& removedItem);
void clearCart(CartLinkedList& list);

// 4.3. Ham dung chung cho Stack (UNDO)
void initStack(UndoStack& s);
bool isEmptyStack(UndoStack& s);
void pushStack(UndoStack& s, UndoAction val);
UndoAction topStack(UndoStack& s);
void popStack(UndoStack& s);
void destroyStack(UndoStack& s);

// 4.4. Ham dung chung cho Queue (Don hang)
void initQueue(OrderQueue& q);
bool isEmptyQueue(OrderQueue& q);
void enqueueOrder(OrderQueue& q, Order val);
Order dequeueOrder(OrderQueue& q);
void destroyQueue(OrderQueue& q);

// 4.5. Ham dung chung cho Cay AVL (Khoi tao, Nap chi muc, Tra cuu, Giai phong)
void initAVL(AVLTree& t);
int heightAVL(AVLNode* node);
int balanceFactor(AVLNode* node);
void updateHeight(AVLNode* node);
AVLNode* createAVLNode(Product val);
AVLNode* rotateRight(AVLNode* node);
AVLNode* rotateLeft(AVLNode* node);
AVLNode* rotateLR(AVLNode* node);
AVLNode* rotateRL(AVLNode* node);
AVLNode* rebalance(AVLNode* node);
AVLNode* addAVLNode(AVLNode* node, Product val);
AVLNode* searchAVL(AVLNode* node, int productId);
void inOrderAVL(AVLNode* root, int& printedCount, int maxPrint);
void destroyAVLNode(AVLNode* node);

// 4.6. Ham dung chung cho Bang bam Voucher (Khoi tao, Bam, Them, Tim, Giai phong)
void initHashTable(VoucherHashTable& ht);
int hashFunctionDivision(int key);
void insertVoucher(VoucherHashTable& ht, int key, string codeName, double value);
VoucherNode* searchVoucher(VoucherHashTable& ht, int key);
void destroyHashTable(VoucherHashTable& ht);

// 4.7. Ham tra cuu mang dung chung (Phuc vu kiem tra ton kho khi Checkout)
int findProductIndexByID(Product catalog[], int n, int productId);

#endif // GIO_HANG_H
