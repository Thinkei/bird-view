const path = require('path');
const outputDir = path.join(__dirname, 'dist/');

const HtmlWebpackPlugin = require('html-webpack-plugin');

const isProd = process.env.NODE_ENV === 'production';

const devServer = {
    contentBase: outputDir,
    historyApiFallback: true
};

module.exports = {
    entry: './src/Index.bs.js',
    mode: isProd ? 'production' : 'development',
    output: {
        path: outputDir,
        publicPath: process.env.PUBLIC_PATH || outputDir,
        filename: isProd ? 'Index-[contenthash].js' : 'Index.js'
    },
    devServer: !isProd ? devServer : undefined,
    plugins: [
        new HtmlWebpackPlugin()
    ],
    module: {
        rules: [
            {
                test: /\.css$/i,
                use: ['style-loader', 'css-loader'],
            },
        ],
    }
};
