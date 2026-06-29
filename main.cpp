#include <bits/stdc++.h>
#include <iostream>
using namespace std;
//enum for type safety
enum Color{
    WHITE, BLACK
};
enum PieceType{
    KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN
};
enum GameStatus{
    WAITING, IN_PROGRESS, COMPLETED, ABORTED
};

//Forward Declarations
class Board;
class Piece;
class Match;
class User;

//Position class to represent coordinates
class Position {
    private: 
    int row;
    int col;

    public:
    Position() {
        row=0;
        col=0;
    }
    Position (int r, int c){
        row=r;
        col=c;
    }
    int getRow() const{
        return row;
    }
    int getCol() const{
        return col;
    }
    bool isValid() const {
        return row>=0 && row<8 && col>=0 && col<8;
    }
    bool operator== (const Position& other) const{
        return row == other.row && col == other.col;
    }
    bool operator< (const Position& other) const{
       if (row!=other.row) return row < other.row;
        return col < other.col;
    }
    string toString() const{
        return "(" + to_string(row) + "," + to_string(col) + ")";
    }
    string toChessNotation() const{
        char file = 'a' + col;
        char rank = '8' - row;
        return string(1, file) + string(1, rank);
    }
};

//Move class to represent a chess move
class Move {
    private:
    Position from; 
    Position to;
    Piece* piece;
    Piece* capturedPiece;
    
    public:
    Move(){
        piece = nullptr;
        capturedPiece= nullptr;
    }
    Move(Position f, Position t, Piece* p, Piece* captured){
        from =f;
        to = t;
        piece = p;
        capturedPiece = captured;
    }
    Position getFrom() const{
        return from;
    }
    Position getTo() const{
        return to;
    }
    Piece* getPiece() const{
        return piece;
    }
    Piece* getCapturedPiece() const{
        return capturedPiece;
    }
};

//Abstract Piece class following Strategy Pattern
class Piece{
protected:
Color color;
PieceType type;
bool hasMoved;

public:
Piece (Color c, PieceType t){
    color = c;
    type = t;
    hasMoved = false;
}
virtual ~Piece() {}
Color getColor() const{
    return color;
}
PieceType getType() const{
    return type;
}
bool getHasMoved() const{
    return hasMoved;
}
void setMoved(bool moved) {
    hasMoved= moved;
}

virtual vector<Position> getPossibleMoves(Position currentPos, Board* board)=0;
virtual string getSymbol() = 0;

string toString(){
    string colorStr = (color == WHITE) ? "W" : "B";
    return colorStr + getSymbol();
}
};

//Concrete Piece Implementations
class King : public Piece{
    public: 
    King(Color color) : Piece (color, KING) {}
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override{
        return "K";
    }
};
class Queen : public Piece{
    public: 
    Queen(Color color) : Piece (color, QUEEN) {}
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override{
        return "Q";
    }
};
class Rook : public Piece{
    public: 
    Rook(Color color) : Piece (color, ROOK) {}
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override{
        return "R";
    }
};
class Bishop : public Piece{
    public: 
    Bishop(Color color) : Piece (color, BISHOP) {}
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override{
        return "B";
    }
};
class Knight : public Piece{
    public: 
    Knight(Color color) : Piece (color, KNIGHT) {}
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override{
        return "N";
    }
};
class Pawn : public Piece{
    public: 
    Pawn(Color color) : Piece (color, PAWN) {}
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override{
        return "P";
    }
};

class PieceFactory{
    public:
    static Piece* createPiece(PieceType type, Color color){
        switch (type){
       
            case KING:   return new King(color);
            case QUEEN:  return new Queen(color);
            case ROOK:   return new Rook(color);
            case BISHOP: return new Bishop(color);
            case KNIGHT: return new Knight(color);
            case PAWN:   return new Pawn(color);
            default:     return nullptr;
        }
    }
};
//Constructor
class Board {
    private:
    Piece* board[8][8];
    map<Position, Piece*> piecePosition;
     public:
     Board(){
        //Initialize board to null
        for (int i = 0; i < 8; i++)
        {
           for (int j = 0; j < 8; j++)
           {
           board[i][j]= nullptr;
           }
        }
        initializeBoard();
     }
//Destructor
     ~Board(){
        //Clean up pieces safely
          for (int i = 0; i < 8; i++)
        {
           for (int j = 0; j < 8; j++)
           {
         if  (board[i][j]!= nullptr){
            delete board[i][j]; 
            board[i][j]= nullptr;
         }
           }
        }
        piecePosition.clear();
     }

void initializeBoard()  {
//White peices

    placePiece(Position(7, 0), PieceFactory::createPiece(ROOK, WHITE));
    placePiece(Position(7, 1), PieceFactory::createPiece(KNIGHT, WHITE));
    placePiece(Position(7, 2), PieceFactory::createPiece(BISHOP, WHITE));
    placePiece(Position(7, 3), PieceFactory::createPiece(QUEEN, WHITE));
    placePiece(Position(7, 4), PieceFactory::createPiece(KING, WHITE));
    placePiece(Position(7, 5), PieceFactory::createPiece(BISHOP, WHITE));
    placePiece(Position(7, 6), PieceFactory::createPiece(KNIGHT, WHITE));
    placePiece(Position(7, 7), PieceFactory::createPiece(ROOK, WHITE));

    for (int i = 0; i < 8; i++) {
        placePiece(Position(6, i), PieceFactory::createPiece(PAWN, WHITE));
    }

    // Initialize black pieces
    placePiece(Position(0, 0), PieceFactory::createPiece(ROOK, BLACK));
    placePiece(Position(0, 1), PieceFactory::createPiece(KNIGHT, BLACK));
    placePiece(Position(0, 2), PieceFactory::createPiece(BISHOP, BLACK));
    placePiece(Position(0, 3), PieceFactory::createPiece(QUEEN, BLACK));
    placePiece(Position(0, 4), PieceFactory::createPiece(KING, BLACK));
    placePiece(Position(0, 5), PieceFactory::createPiece(BISHOP, BLACK));
    placePiece(Position(0, 6), PieceFactory::createPiece(KNIGHT, BLACK));
    placePiece(Position(0, 7), PieceFactory::createPiece(ROOK, BLACK));

    for (int i = 0; i < 8; i++) {
        placePiece(Position(1, i), PieceFactory::createPiece(PAWN, BLACK));
    }
}

 void placePiece(Position pos, Piece* piece){
board [pos.getRow() ][pos.getCol()] = piece;
piecePosition[pos] = piece;
 }
 void removePiece(Position pos){
    board [pos.getRow() ][pos.getCol()] = nullptr;
piecePosition.erase(pos);
 }
 Piece* getPiece(Position pos){
   return board[pos.getRow()][pos.getCol()];
 }
 bool isOccupied (Position pos){
     return getPiece(pos)!= nullptr;
 }
 bool isOccupiedBySameColor (Position pos, Color color){
    Piece* piece = getPiece(pos);
    return piece!= nullptr && piece->getColor() == color;
 }
void movePiece (Position from, Position to){
    Piece* piece = getPiece(from);
    if (piece!=nullptr){
        Piece* capturedPiece = getPiece(to);
    if(capturedPiece != nullptr){
delete capturedPiece;
piecePosition.erase(to);
    }

    //Moving Piece
board[from.getRow()][from.getCol()]= nullptr;
board[to.getRow()][to.getCol()] = piece;
//Update map
piecePosition.erase(from);
piecePosition[to]= piece;

//due to pawn movement
piece->setMoved(true);
}
}
Position findKing (Color color){
     for (auto& pair: piecePosition){
        if(pair.second->getType()==KING && pair.second->getColor()==color){
            return pair.first;
        }
    }
    return Position(-1,-1); //Invalid 
}
vector<Position>getAllPiecesOfColor(Color color){
    vector<Position> pieces;
     for (auto& pair: piecePosition){
        if (pair.second->getColor()==color){
            pieces.push_back(pair.first);
        }
    }
    return pieces;
}

  void display() {
        constexpr int cellW = 3;  // cell width

        // — horizontal border —
        auto printBorder = [&]() {
            std::cout << "  +";
            for (int i = 0; i < 8; ++i)
                std::cout << std::string(cellW, '-') << "+";
            std::cout << "\n";
        };

        // — top border —
        printBorder();

        // — column labels inside the grid —
        std::cout << "  |";
        for (char f = 'a'; f <= 'h'; ++f) {
            int pad = (cellW - 1) / 2;
            std::cout
                << std::string(pad, ' ')
                << f
                << std::string(cellW - 1 - pad, ' ')
                << "|";
        }
        std::cout << "\n";

        // — border under labels —
        printBorder();

        // — each rank of pieces —
        for (int rank = 8; rank >= 1; --rank) {
            int row = 8 - rank;
            std::cout << rank << " |";

            for (int file = 0; file < 8; ++file) {
                Piece* p = board[row][file];
                std::string s = p ? p->toString() : "  ";  // two spaces if empty

                // center a 2-char string in cellW
                int pad = (cellW - 2) / 2;
                std::cout
                    << std::string(pad, ' ')
                    << s
                    << std::string(cellW - 2 - pad, ' ')
                    << "|";
            }

            std::cout << " " << rank << "\n";
            printBorder();
        }

        // — bottom labels inside the grid —
        std::cout << "  |";
        for (char f = 'a'; f <= 'h'; ++f) {
            int pad = (cellW - 1) / 2;
            std::cout
                << std::string(pad, ' ')
                << f
                << std::string(cellW - 1 - pad, ' ')
                << "|";
        }
        std::cout << "\n";

        // — final border —
        printBorder();
    }

};
// Now implement the piece movement methods
vector<Position> King::getPossibleMoves(Position currentPos, Board* board) {
    vector<Position> moves;
    int directions[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    
    for (int i = 0; i < 8; i++) {
        Position newPos(currentPos.getRow() + directions[i][0], currentPos.getCol() + directions[i][1]);
        if (newPos.isValid() && !board->isOccupiedBySameColor(newPos, this->color)) {
            moves.push_back(newPos);
        }
    }
    return moves;
}

vector<Position> Queen::getPossibleMoves(Position currentPos, Board* board) {
    vector<Position> moves;
    int directions[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    
    for (int d = 0; d < 8; d++) {
        for (int i = 1; i < 8; i++) {
            Position newPos(currentPos.getRow() + directions[d][0]*i, currentPos.getCol() + directions[d][1]*i);
            if (!newPos.isValid()) break;

            if (board->isOccupiedBySameColor(newPos, this->color)) break;

            moves.push_back(newPos);
            if (board->isOccupied(newPos)) break; // Stop after capturing
        }
    }
    return moves;
}

vector<Position> Rook::getPossibleMoves(Position currentPos, Board* board) {
    vector<Position> moves;
    int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    
    for (int d = 0; d < 4; d++) {
        for (int i = 1; i < 8; i++) {
            Position newPos(currentPos.getRow() + directions[d][0]*i, currentPos.getCol() + directions[d][1]*i);
            if (!newPos.isValid()) break;

            if (board->isOccupiedBySameColor(newPos, this->color)) break;

            moves.push_back(newPos);
            if (board->isOccupied(newPos)) break;
        }
    }
    return moves;
}

vector<Position> Bishop::getPossibleMoves(Position currentPos, Board* board) {
    vector<Position> moves;
    int directions[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};
    
    for (int d = 0; d < 4; d++) {
        for (int i = 1; i < 8; i++) {
            Position newPos(currentPos.getRow() + directions[d][0]*i, currentPos.getCol() + directions[d][1]*i);
            if (!newPos.isValid()) break;
            if (board->isOccupiedBySameColor(newPos, this->color)) break;
            moves.push_back(newPos);
            if (board->isOccupied(newPos)) break;
        }
    }
    return moves;
}

vector<Position> Knight::getPossibleMoves(Position currentPos, Board* board) {
    vector<Position> moves;
    int knightMoves[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    
    for (int i = 0; i < 8; i++) {
        Position newPos(currentPos.getRow() + knightMoves[i][0], currentPos.getCol() + knightMoves[i][1]);
        if (newPos.isValid() && !board->isOccupiedBySameColor(newPos, this->color)) {
            moves.push_back(newPos);
        }
    }
    return moves;
}

vector<Position> Pawn::getPossibleMoves(Position currentPos, Board* board) {
    vector<Position> moves;
    int direction = (color == WHITE) ? -1 : 1;
    
    // Forward move
    Position oneStep(currentPos.getRow() + direction, currentPos.getCol());
    if (oneStep.isValid() && !board->isOccupied(oneStep)) {
        moves.push_back(oneStep);
        
        // Double move from starting position
        if (!hasMoved) {
            Position twoStep(currentPos.getRow() + 2*direction, currentPos.getCol());
            if (twoStep.isValid() && !board->isOccupied(twoStep)) {
                moves.push_back(twoStep);
            }
        }
    }
    
    // Diagonal captures
    Position leftCapture(currentPos.getRow() + direction, currentPos.getCol() - 1);
    Position rightCapture(currentPos.getRow() + direction, currentPos.getCol() + 1);
    
    if (leftCapture.isValid() && board->isOccupied(leftCapture) && 
        !board->isOccupiedBySameColor(leftCapture, this->color)) {
        moves.push_back(leftCapture);
    }
    
    if (rightCapture.isValid() && board->isOccupied(rightCapture) && 
        !board->isOccupiedBySameColor(rightCapture, this->color)) {
        moves.push_back(rightCapture);
    }
    
    return moves;
}
//Rule book

class ChessRules {
public:
virtual ~ChessRules() {}
virtual bool isValidMove(Move move, Board* board) =0;
virtual bool isInCheck(Color color, Board* board) =0;
virtual bool isCheckmate(Color color, Board* board) =0;
virtual bool isStalemate(Color color, Board* board) =0;
virtual bool wouldMoveCauseCheck(Move move,  Board* board, Color kingColor) =0;

};

class StandardChessRules : public ChessRules{
    public:
    bool isValidMove(Move move, Board* board) override{
Piece* piece = move.getPiece();
vector<Position> possibleMoves = piece->getPossibleMoves(move.getFrom(),board);
//Check if taget pos is in possible pos
bool validDestination = false;
for( const Position& pos: possibleMoves){
    if (pos==move.getTo()){
        validDestination= true;
        break;
    }
}
if(!validDestination){
    return false;
}
return !wouldMoveCauseCheck(move, board, piece->getColor());
    }

bool wouldMoveCauseCheck( Move move, Board* board, Color kingColor) override{
    //making a copy
    Piece* movingPiece = board->getPiece(move.getFrom());
    Piece* capturedPiece = board->getPiece(move.getTo());
if (movingPiece==nullptr) return true; //invalid move gng
//temporary move execution
board->removePiece(move.getFrom());
if(capturedPiece!= nullptr){
    board->removePiece(move.getTo());
}
board->placePiece(move.getTo(), movingPiece);
//checking upon on king
bool inCheck = isInCheck(kingColor, board);
//undo
board->removePiece(move.getTo());
   board->placePiece(move.getFrom(), movingPiece);
if (capturedPiece!= nullptr){
   board->placePiece(move.getTo(), capturedPiece);
}
return inCheck;
}

bool isInCheck(Color color, Board* board) override {
    Position kingPos = board ->findKing(color);
    if(kingPos.getRow()==-1) return false; //king missing
    Color opponentColor = (color==WHITE) ? BLACK : WHITE;
    vector<Position> opponentPieces = board->getAllPiecesOfColor(opponentColor);

    for(const Position& pos: opponentPieces){
        Piece* piece = board->getPiece(pos);
 vector<Position> moves= piece->getPossibleMoves(pos,board);
 for(const Position& targetPos : moves){
    if(targetPos==kingPos){
        return true;
    }
 }
    }
    return false;
}

bool isCheckmate(Color color, Board* board) override {
        if (!isInCheck(color, board)) return false;
        
        vector<Position> pieces = board->getAllPiecesOfColor(color);
        for (const Position& pos : pieces) {
            Piece* piece = board->getPiece(pos);
            vector<Position> moves = piece->getPossibleMoves(pos, board);
            
            for (const Position& targetPos : moves) {
                Move move(pos, targetPos, piece, board->getPiece(targetPos));
                if (isValidMove(move, board)) {
                    return false; // Found a valid move, not checkmate
                }
            }
        }
        return true;
    }
  bool isStalemate(Color color, Board* board) override {
        if (isInCheck(color, board)) return false;
        
        vector<Position> pieces = board->getAllPiecesOfColor(color);
        for (const Position& pos : pieces) {
            Piece* piece = board->getPiece(pos);
            vector<Position> moves = piece->getPossibleMoves(pos, board);
            
            for (const Position& targetPos : moves) {
                Move move(pos, targetPos, piece, board->getPiece(targetPos));
                if (isValidMove(move, board)) {
                    return false; // Found a valid move, not stalemate
                }
            }
        }
        return true;
    }
};

//Message class for chatting
class Message{
private:
string senderId;
string content;
time_t timestamp;

public:
Message(string sId, string msg){
    senderId = sId;
    content =msg;
    timestamp= time(0);
}
string getSenderId() const{
    return senderId;
}
string getContent() const{
    return content;
}
time_t getTimetsamp(){
    return timestamp;
}
string toString() const{
    return "[" +senderId + "]: " + content; 
}
};

//Mediator Pattern- Interface
class ChatMediator {
    public:
virtual ~ChatMediator(){}
  virtual void sendMessage(Message* message, User* user) = 0;
    virtual void addUser(User* user) = 0;
    virtual void removeUser(User* user) = 0;

};

class Colleague {
    protected: 
    ChatMediator* mediator;

    public:
    Colleague(): mediator(nullptr){}
    virtual ~Colleague(){}
        virtual void send(Message* message) = 0;
        virtual void receive(Message* message) = 0;
    virtual void setMediator(ChatMediator* med){
        mediator = med;
    }
ChatMediator* getMediator() const{
    return mediator;
}
};
// User class ingeriting from colleague 
class User : public Colleague{
private:
string id;
string name;
int score;

public:
User(string userId, string userName) : Colleague(){
    id = userId;
    name= userName;
    score=1000; //initial score
}
string getId() const{
    return id;
}
string getName() const{
    return name;
}
int getScore() const{
    return score;
}
   void incrementScore(int points) {
        score += points;
    }
    
    void decrementScore(int points) {
        score -= points;
    }
    string toString() const{
        return name + " (Score: " + to_string(score) + ")";
    }
    // colleague interface
    void send(Message* message) override{
        if (mediator!=nullptr){
            mediator->sendMessage(message, this);
        }
    }
    void receive(Message* message) override{
        cout << "User " << name << " received message from " << message->getSenderId() << ": " << message->getContent() << endl;
    }
};

class Match :  public ChatMediator{
    private:
    string matchId;
    User* whitePlayer;
    User* blackPlayer;
    Board* board;
    ChessRules* rules;
    Color currentTurn;
    GameStatus status;
    vector<Move> moveHistory;
    vector<Message*> chatHistory;

public:

Match(string mId, User* white, User* black){
    matchId = mId;
    whitePlayer= white;
    blackPlayer = black;
    board = new Board();
    rules = new StandardChessRules();
    currentTurn = WHITE;
    status= IN_PROGRESS;

    //setting mediator for both users
    whitePlayer-> setMediator(this);
    blackPlayer->setMediator(this);

    cout<<"Match started between " << whitePlayer->getName() << " (White) and " << blackPlayer->getName()<< " (Black)" << "\n";
}
~Match(){
    delete board;
    delete rules;
}
bool makeMove(Position from, Position to, User* player){
    if (status!= IN_PROGRESS){
        cout<<"Please let the game start :)"<< "\n";
        return false;
    }
    Color playerColor = getPlayerColor(player);
    if (playerColor!= currentTurn){
        cout<< "Please wait for your turn :)"<<"\n";
        return false;
    }
    Piece* piece = board->getPiece(from);
    if (piece==nullptr|| piece->getColor()!= playerColor){
        cout<< "This is not your piece :)"<<"\n";
        return false;
    }
       Move move(from, to, piece, board->getPiece(to));

    if (!rules->isValidMove(move,board)){
        cout<<"Move is not valid"<<"\n";
        return false;
    }

    //Executing move
    board->movePiece(from,to);
    moveHistory.push_back(move);

    cout<< player->getName()<<" moved "<< piece->getSymbol() << " from " << from.toChessNotation()<< " to " << to.toChessNotation()<< "\n";
     board->display();
     //Check if game ended
     Color opponentColor = (currentTurn==WHITE) ? BLACK : WHITE;
     if(rules->isCheckmate(opponentColor, board)){
        endGame(player, "Checkmate");
        return true;
     }
  else {
            currentTurn = opponentColor;
            if (rules->isInCheck(opponentColor, board)) {
                cout << getPlayerByColor(opponentColor)->getName() << " is in check!" << endl;
            }
        }
        return true;
    }
    void undoMove() {

    if (moveHistory.empty()) {
        cout << "No moves to undo.\n";
        return;
    }

    Move lastMove = moveHistory.back();
    moveHistory.pop_back();

    Piece* piece = board->getPiece(lastMove.getTo());

    board->removePiece(lastMove.getTo());
    board->placePiece(lastMove.getFrom(), piece);

    if (lastMove.getCapturedPiece() != nullptr) {
        board->placePiece(lastMove.getTo(), lastMove.getCapturedPiece());
    }

    currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;

    cout << "Last move undone.\n";
    board->display();
}

    void quitGame(User* player){
        User* opponent = (player == whitePlayer)? blackPlayer : whitePlayer;
        endGame(opponent, "Quit");
        player->decrementScore(70); //quitting penalty
        cout<< player->getName()<< " quit the game. Penalty of 70 applied. Bleghh"<<"\n";
    }
     void endGame(User* winner, string reason){
        status = COMPLETED;
        if (winner!= nullptr){
            User* loser = (winner == whitePlayer) ? blackPlayer : whitePlayer;
            winner->incrementScore(40);
            loser-> decrementScore(30);
            cout<<"Game ended- " << winner->getName()<< " wins by "<< reason << ". Wohoo!" << "\n";
            cout<< "Scores: " << winner->getName() << " +40, "<< loser->getName() << " -30"<< "\n";

        }
        else{
            cout<< "Game ended in " << reason<<". No score changes."<<"\n";
         }
     }
     Color getPlayerColor(User* player){
        return (player==blackPlayer) ? BLACK : WHITE;
     }
       User* getPlayerByColor(Color color) {
        return (color == WHITE) ? whitePlayer : blackPlayer;
    }
    //mediator pattern
    void sendMessage(Message* message, User* user) override {
        chatHistory.push_back(message);
        
        User* recipient = (user == whitePlayer) ? blackPlayer : whitePlayer;
        recipient->receive(message);
        cout << "Chat in match " << matchId << " - " << message->getContent() << endl;
    }
    void addUser(User* user) override{
        //not applicable, chess can onlt have 2 players
    }
    void removeUser (User* user) override{
        quitGame(user);
    }
    string getMAtchId() const{
        return matchId;
    }
      GameStatus getStatus() const { 
        return status; 
    }
    User* getWhitePlayer() const { 
        return whitePlayer; 
    }
    User* getBlackPlayer() const { 
        return blackPlayer; 
    }
    Board* getBoard() const { 
        return board; 
    }
    Color getCurrentTurn() const {
    return currentTurn;
}
};
//Matching strategy
class MatchingStrategy{
    public:
    virtual ~MatchingStrategy(){}
    virtual User* findMatch(User* user, vector <User*> & waitingUser) =0;
};
//on basis of score

class ScoreBAsedMatching : public  MatchingStrategy{
private: int scoreTolerance;
public:
ScoreBAsedMatching(int tolerance){
    scoreTolerance= tolerance;
}
User* findMatch(User* user, vector<User*>& waitingUsers) override{
    User* bestMatch = nullptr;
    int bestScoreDiff= INT32_MAX;

    for (User* waitingUser: waitingUsers){
        if(waitingUser->getId()!= user->getId()){
            int scoreDiff = abs(waitingUser->getScore()-user->getScore());
            if (scoreDiff<=scoreTolerance && scoreDiff< bestScoreDiff){
                bestMatch = waitingUser;
                bestScoreDiff= scoreDiff;
            }
        }
    }
    return bestMatch;
}
};

//Game Manager- Singleton Pattern
class GameManager{
    private:
static GameManager* instance;
map<string, Match*> activeMatches; //matchId-->match
vector<User*> waitingUsers;
MatchingStrategy* matchingStrategy;
int matchCounter;

GameManager(){
    matchingStrategy=new ScoreBAsedMatching(100);//tolerance of 100 points
    matchCounter=0;

}
public:
static GameManager* getInstance(){
    if (instance==nullptr){
        instance = new GameManager();
    }
    return instance;
}
  ~GameManager() {
        delete matchingStrategy;
        for (auto& pair : activeMatches) {
            delete pair.second;
        }
    }
     void requestMatch(User* user){
        cout<< user->getName()<< " is looking for a match..." << "\n";
        User* opponent = matchingStrategy->findMatch(user, waitingUsers);
        if (opponent!=nullptr){
            waitingUsers.erase(remove(waitingUsers.begin(),waitingUsers.end(),opponent ), waitingUsers.end());
     string matchId = "MATCH_" + to_string(++matchCounter);
     Match* match = new Match(matchId, user, opponent);
     activeMatches[matchId]= match;
      cout << "Match found! " << user->getName() << " vs " << opponent->getName() << endl;
       match->getBoard()->display();
     
        }
        else{
            waitingUsers.push_back(user);
                cout << user->getName() << " added to waiting list." << endl;
        }
     }

     void makeMove(string matchId, Position from, Position to, User* player){
        if (activeMatches.find(matchId)!= activeMatches.end()){
            Match* match = activeMatches[matchId];
            match->makeMove(from,to,player);
            if(match ->getStatus()==COMPLETED){
                delete match;
                activeMatches.erase(matchId);
                cout<<"Match " << matchId << " completed and removed from active matches. \n";
 
            }
        }
     }

     void quitMatch(string matchId, User* player){
         if (activeMatches.find(matchId)!= activeMatches.end()){
            Match* match = activeMatches[matchId];
              match->quitGame(player);
            delete match;
            activeMatches.erase(matchId);
     }
    }
       void sendChatMessage(string matchId, string message, User* user) {
        if (activeMatches.find(matchId) != activeMatches.end()) {
            Match* match = activeMatches[matchId];
            Message* msg = new Message(user->getId(), message);
            match->sendMessage(msg, user);
            delete msg;
        }
    }
    
    Match* getMatch(string matchId) {
        if (activeMatches.find(matchId) != activeMatches.end()) {
            return activeMatches[matchId];
        }
        return nullptr;
    }
    void displayActiveMatches(){
        cout<< "\n=== Active Matches ===\n";
        for (auto& pair: activeMatches){
            Match* match = pair.second;
            cout << "Match " << match->getMAtchId() << ": " 
                 << match->getWhitePlayer()->getName() << " vs " 
                 << match->getBlackPlayer()->getName() << endl;
        }   cout << "Total active matches: " << activeMatches.size() << endl;
        cout << "Users waiting: " << waitingUsers.size() << endl;
        }
};
//Initializing static member
GameManager* GameManager::instance = nullptr;

//DEMO CLASS WITH SCHOLAR'S MATE
class ChessSystemDemo {
public:
    // Method to demonstrate Scholar's Mate (4-move checkmate)
    static void demonstrateScholarsMate() {
        cout << "\n=== Scholar's Mate Demo (4-move checkmate) ===" << endl;
        
        User* aditya = new User("DEMO_1", "Aditya");
        User* rohit = new User("DEMO_2", "Rohit");
        
        Match* demoMatch = new Match("DEMO_MATCH", aditya, rohit);
        demoMatch->getBoard()->display();
        
        // Proper Scholar's Mate sequence with correct coordinates
        cout << "\nMove 1: White e2-e4" << endl;
        demoMatch->makeMove(Position(6, 4), Position(4, 4), aditya); // e2-e4
        
        cout << "\nMove 1: Black e7-e5" << endl;
        demoMatch->makeMove(Position(1, 4), Position(3, 4), rohit); // e7-e5
        
        cout << "\nMove 2: White Bf1-c4 (targeting f7)" << endl;
        demoMatch->makeMove(Position(7, 5), Position(4, 2), aditya); // Bf1-c4
        
        cout << "\nMove 2: Black Nb8-c6 (developing)" << endl;
        demoMatch->makeMove(Position(0, 1), Position(2, 2), rohit); // Nb8-c6
        
        cout << "\nMove 3: White Qd1-h5 (attacking f7 and h7)" << endl;
        demoMatch->makeMove(Position(7, 3), Position(3, 7), aditya); // Qd1-h5 (row 3, col 7 = h5)
        
        cout << "\nMove 3: Black Ng8-f6?? (defending h7 but exposing f7)" << endl;
        demoMatch->makeMove(Position(0, 6), Position(2, 5), rohit); // Ng8-f6
        
        cout << "\nMove 4: White Qh5xf7# (Checkmate!)" << endl;
        bool gameEnded = demoMatch->makeMove(Position(3, 7), Position(1, 5), aditya); // Qh5xf7#
        
        if (demoMatch->getStatus() != COMPLETED) {
            cout << "Note: Checkmate detection may need refinement for this position." << endl;
        }
        
        // Demonstrate chat functionality
        cout << "\n=== Testing Chat Functionality ===" << endl;
        aditya->send(new Message(aditya->getId(), "Good game!"));
        rohit->send(new Message(rohit->getId(), "Thanks, that was a quick one!"));
        
        // Clean up properly
        delete demoMatch; // This will handle board and rules cleanup
        delete aditya;
        delete rohit;
    }
};
// Helper function to convert chess notation to Position
Position chessToPosition(string s) {
    int col = s[0] - 'a';
    int row = '8' - s[1];
    return Position(row, col);
}

void playChess() {

    string whiteName, blackName;

    cout << "Enter White Player Name: ";
    cin >> whiteName;

    cout << "Enter Black Player Name: ";
    cin >> blackName;

    User* white = new User("P1", whiteName);
    User* black = new User("P2", blackName);

    Match match("GAME_1", white, black);

    match.getBoard()->display();

    string from, to;

    while (match.getStatus() == IN_PROGRESS) {

        if (match.getCurrentTurn() == WHITE)
            cout << "\nWhite (" << white->getName()
                 << ") move (or type 'undo'): ";
        else
            cout << "\nBlack (" << black->getName()
                 << ") move (or type 'undo'): ";

        cin >> from;

        // Undo last move
        if (from == "undo") {
            match.undoMove();
            continue;
        }

        cin >> to;

        Position start = chessToPosition(from);
        Position end = chessToPosition(to);

        if (!start.isValid() || !end.isValid()) {
            cout << "Invalid move format! Use e2 e4\n";
            continue;
        }

        User* currentPlayer;

        if (match.getCurrentTurn() == WHITE)
            currentPlayer = white;
        else
            currentPlayer = black;

        match.makeMove(start, end, currentPlayer);
    }

    delete white;
    delete black;
}
void gameManagerDemo() {

    GameManager* manager = GameManager::getInstance();

    int n;

    cout << "\nHow many players? ";
    cin >> n;

    vector<User*> players;

    for (int i = 0; i < n; i++) {

        string name;

        cout << "Enter Player " << i + 1 << " Name: ";
        cin >> name;

        User* player = new User("P" + to_string(i + 1), name);

        players.push_back(player);
    }

    cout << "\nSearching for matches...\n\n";

    for (User* player : players)
        manager->requestMatch(player);

    manager->displayActiveMatches();
}
void chatDemo() {

    string name1, name2;

    cout << "Enter White Player Name: ";
    cin >> name1;

    cout << "Enter Black Player Name: ";
    cin >> name2;

    User* white = new User("P1", name1);
    User* black = new User("P2", name2);

    Match match("CHAT_MATCH", white, black);

    cout << "\nType 'exit' to end chat.\n";

    while (true) {

        string message;

        cout << "\n" << white->getName() << ": ";

        getline(cin >> ws, message);

        if (message == "exit")
            break;

        white->send(new Message(white->getId(), message));

        cout << "\n" << black->getName() << ": ";

        getline(cin >> ws, message);

        if (message == "exit")
            break;

        black->send(new Message(black->getId(), message));
    }

    delete white;
    delete black;
}

//Main function
int main() {

    cout << "====== STRATAGEM ======\n";

    while (true) {

        cout << "\n1. Play Chess\n";
        cout << "2. Scholar's Mate Demo\n";
        cout << "3. Game Manager Demo\n";
        cout << "4. Game Chat Demo\n";
        cout << "5. Exit\n";

        cout << "\nEnter Choice: ";

        int choice;
        cin >> choice;

        switch(choice){

        case 1:
            playChess();
            break;

        case 2:
            ChessSystemDemo::demonstrateScholarsMate();
            break;

        case 3:
             gameManagerDemo();
            break;

        case 4:
        chatDemo();
           break;

            case 5: 
            return 0;
        default:
            cout << "Invalid Choice\n";
        }
    }
}
