FROM ubuntu:latest
RUN apt-get update && apt-get install -y g++ cmake make libasio-dev
WORKDIR /app
COPY . .
RUN g++ -O3 main.cpp -o chess_engine -lpthread
EXPOSE 18080
CMD ["./chess_engine"]
