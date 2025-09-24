FROM node:20

WORKDIR /app

COPY web-server.js ./

RUN npm init -y
RUN npm install ws

EXPOSE 8080

CMD ["node", "web-server.js"]

